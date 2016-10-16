#ifndef __PAD_H__
#define __PAD_H__

#include <avr/io.h>
#include "ssd1306.hpp"

typedef struct {
	uint8_t x, y;
} point8_t;

typedef struct {
	int16_t x, y;
} point16_t;

class Pad {
public:
	Pad(uint8_t xPos);
	int16_t getX();
	int16_t getY();
	int16_t getVel();
	void setY(uint16_t yPos);
	void refresh(SSD1306& display);
private:
	point16_t pos;
	int8_t avgVel;
	int8_t points;
};

#endif