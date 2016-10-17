
#include "ball.hpp"
Ball::Ball() : pos(), vel(), spin(0) {
	pos.x = PIX_SCL*128/2;
	pos.y = PIX_SCL*64/2;
	vel.x = BALL_INIT_SPEED;
	vel.y = 0;
}

void Ball::setX(uint8_t x) {
	pos.x = x*PIX_SCL;
}

void Ball::setY(uint8_t y) {
	pos.y = y*PIX_SCL;
}

void Ball::setVelX(int16_t velX) {
	vel.x = velX;
}

void Ball::setVelY(int16_t velY) {
	vel.y = velY;
}

int16_t Ball::getX() {
	return pos.x/PIX_SCL;
}

int16_t Ball::getY() {
	return pos.y/PIX_SCL;
}

point16_t Ball::getVel() {
	return vel;
}

void Ball::revX() {
	vel.x *= -1;
}

uint8_t Ball::bouncePad(Pad& pad) {
	point16_t posPad = {pad.getX(), pad.getY()};
	posPad.y -= 4;
	int16_t x = getX(), y = getY();
	// Change direction if touching pad
	if (x-posPad.x == 0) {
		int16_t padVel = pad.getVel();
		uint8_t absPadVel = 0;//(padVel<0?-padVel:padVel);
		if (y-posPad.y < 8+absPadVel && y-posPad.y+absPadVel >= 0) {
			vel.x *= -1;
			if (vel.x > 0) {
				setX(posPad.x + 1);
				spin -= pad.getVel()*512/SPEED_SCL; // Spin inwards
			} else if (vel.x < 0) {
				setX(posPad.x - 1);
				spin += pad.getVel()*512/SPEED_SCL; // Spin inwards
			}
			// Velocity to add to ball. Hardcoded values from testing.
			int16_t dvel;
			switch (y-posPad.y) {
				case 0: case 7:
					dvel = 128*64/SPEED_SCL;
					break;
				case 1: case 6:
					dvel = 64*64/SPEED_SCL;
					break;
				case 2: case 5:
					dvel = 16*64/SPEED_SCL;
					break;
				default:
					dvel = 2*64/SPEED_SCL;
			}
			if (y-posPad.y < 4) {
				dvel *= -1;
			}
			// Weighted average between collision position and pad velocity
			vel.y = (vel.y*3 + dvel)/4;
			vel.y += pad.getVel()*512/SPEED_SCL;
			return true;
		}
	}
	return false;
}

void Ball::step() {
	spin = spin - spin/128/SPEED_SCL;
	
	//int16_t dVx = vel.y*spin/128;
	//int16_t dVy = -vel.x*spin/128;
	
	//vel.x += dVx; vel.x -= dVy;
	//vel.y += dVy; vel.y -= dVx;
	
	int16_t dv = spin/16/SPEED_SCL;
	vel.y += (vel.x > 0)?dv:-dv;
	vel.y = vel.y - vel.y/64/SPEED_SCL;
	
	pos.x += vel.x/64;
	pos.y += vel.y/64;
}