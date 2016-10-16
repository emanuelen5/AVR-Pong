
#include "Pong.hpp"

uint8_t i = 0;

Pong::Pong() :
	b(), lPad(0), rPad(127), display() {
}

void Pong::refreshPads() {
	lPad.setY(readADC(PONG_L_PIN)); // [0-255]
	rPad.setY(readADC(PONG_R_PIN)); // [0-255]
	
	lPad.refresh(display);
	rPad.refresh(display);
}

void Pong::stepBall() {
	b.step();
	b.bouncePad(lPad);
	b.bouncePad(rPad);
	b.touchWalls(*this);
}

void Pong::refreshBall() {
	point8_t pos;
	cli();
	pos.x = b.getX();
	pos.y = b.getY();
	sei();

	display.clear_pixel(lastPos.x, lastPos.y);
	display.set_pixel(pos.x, pos.y);
	display.refresh(lastPos.x, lastPos.y);
	display.refresh(pos.x, pos.y);
	lastPos = pos;
}

void Pong::menu() {
	display.clear();
	display.writeStr("Welcome to PONG", 0, 0);
	display.writeStr("Made by Emaus", 0, 55);
	display.refresh();	_delay_ms(3000);
	display.clear();
	display.refresh();
	if (randVal() & 1)
		b.revX(); // Randomize starting direction of ball
}

void setSubString(char *str, const char *subStr, uint8_t pos) {
	for (uint8_t i=0; subStr[i]; i++) {
		str[i+pos] = subStr[i];
	}
}

void Pong::pointMenu() {
	// Place ball depending on who made the point
	int8_t p = 0;
	if (lPoints & 0x80) {
		p = -1;
		lPoints &= 0x7F;
		b.setX(1);
		b.setVelX(BALL_INIT_SPEED);
	} else if (rPoints & 0x80) {
		p = 1;
		rPoints &= 0x7F;
		b.setX(126);
		b.setVelX(-BALL_INIT_SPEED);
	} else {
		b.setX(64);
		b.setVelX(BALL_INIT_SPEED);
	}
	
	display.clear();
	char scoreStr[] = "Scored by 0     ";
	#define SCORE_START_POS 10
	if (p < 0)
		setSubString(scoreStr, "LEFT!", SCORE_START_POS);
	else
		setSubString(scoreStr, "RIGHT!", SCORE_START_POS);
	display.writeStr(scoreStr, 0, 0);

	char pointString[] = "   -   ";
	if (lPoints < 10) {
		pointString[1] = '0' + lPoints;
	} else {
		pointString[0] = '0' + lPoints/10;
		pointString[1] = '0' + lPoints%10;
	}
	if (lPoints < 10) {
		pointString[5] = '0' + rPoints;
	} else {
		pointString[5] = '0' + rPoints/10;
		pointString[6] = '0' + rPoints%10;
	}
	
	display.writeStr(pointString, 60 - 4*3, 28);
	display.refresh(); _delay_ms(3000);
	display.clear();
	display.refresh();
}

void Pong::drawBoundaries() {
	display.line(0, 0, SSD1306_LCDWIDTH-1, 0, 0);
	display.line(0, SSD1306_LCDHEIGHT-1, SSD1306_LCDWIDTH-1, SSD1306_LCDHEIGHT-1, 0);
}

int8_t Pong::madePoint(int8_t l_rn) {
	if (l_rn > 0 && !madePoint(0)) {
		lPoints++;
		lPoints |= BV(7 + 8*(sizeof(lPoints)-1));
	} else if (l_rn < 0 && !madePoint(0)) {
		rPoints++;
		rPoints |= BV(7 + 8*(sizeof(rPoints)-1));
	}
	
	if (lPoints & 0x80) {
		return -1;
	} else if (rPoints & 0x80) {
		return 1;
	} else {
		return 0;
	}
}

uint8_t Ball::touchWalls(Pong &pong) {
	// Teleport left <-> right
	// TODO: Change this to point system
	if (pos.x < 0) {
		vel.x = -BALL_INIT_SPEED;
		vel.y = 0;
		spin = 0;
		pong.madePoint(-1);
		} else if (pos.x >= 128*PIX_SCL) {
		vel.x = BALL_INIT_SPEED;
		vel.y = 0;
		spin = 0;
		pong.madePoint(1);
	}
	
	// Bounce on top/bottom
	if (pos.y < 0) {
		pos.y = -pos.y;
		vel.y *= -1;
		return true;
	} else if (pos.y >= 64*PIX_SCL) {
		pos.y -= pos.y%(64*PIX_SCL);
		vel.y *= -1;
		return true;
	}
	return false;
}