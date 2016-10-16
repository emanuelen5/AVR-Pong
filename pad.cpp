#include "pad.hpp"

Pad::Pad(uint8_t xPos) : 
	pos(), avgVel(0) {
	pos.x = xPos;
	pos.y = 0;
}

int16_t Pad::getX() {
	return pos.x;
}

int16_t Pad::getY() {
	return pos.y >> 4;
}

int16_t Pad::getVel() {
	return avgVel;
}

void Pad::setY(uint16_t yPos) {
	avgVel = (avgVel * 3 + ((int16_t) yPos - pos.y)) / 4;
	pos.y = yPos;
}

void Pad::refresh(SSD1306& display) {
	display.vLine(pos.x,0);
	display.set_block(pos.x, getY()-4, 0xFF);
	display.refresh(pos.x, 0, pos.x, 63);
}