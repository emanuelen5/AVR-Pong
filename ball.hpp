#ifndef __BALL_H__
#define __BALL_H__

class Pong;

#include <avr/io.h>
#include <avr/interrupt.h>
#include "pad.hpp"
#define PIX_SCL 128
#define SPEED_SCL 3
#define BALL_INIT_SPEED 8192/SPEED_SCL

class Ball{
public:
	Ball();
	
	/** Gets the pixel position of the ball by removing decimal part of position
	@return Pixel position of ball
	**/
	int16_t getX();
	int16_t getY();
	
	void setX(uint8_t x);
	void setY(uint8_t y);
	
	point16_t getVel();
	void setVelX(int16_t velX);
	void setVelY(int16_t velY);
	void revX();
	
	uint8_t bouncePad(Pad &pad);
	
	void step();
	
	// Declared by classes which call these functions (system dependent)
	uint8_t touchWalls(Pong &pong);

private:
	// Current position
	point16_t pos;
	// Speed
	point16_t vel;
	// Spin of ball
	int16_t spin;
};

#endif