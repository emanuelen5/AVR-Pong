#ifndef __PONG_H__
#define __PONG_H__

#include <avr/io.h>

#include "ball.hpp"
#include "pad.hpp"
#include "ssd1306.hpp"
#include "main.hpp"

#define PONG_R_PIN 5
#define PONG_L_PIN 4

class Pong {
public:
	Pong();
	void refreshPads();
	void stepBall();
	void refreshBall();
	void menu();
	void pointMenu();
	void drawBoundaries();
	/** 
	* @param l_rn Negative if left scored, positive if right scored. Zero to just return if someone made a point since last time;
	* @return Negative value if left pad scored, positive if right
	*/
	int8_t madePoint(int8_t l_rn);
private:
	Ball b;
	Pad lPad, rPad;
	SSD1306 display;
	point8_t lastPos;
	int8_t lPoints, rPoints; // Last bit is used to check if points were made since last check
};

#endif