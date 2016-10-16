/*
 * main.h
 *
 * Created: 2016-05-08 14:18:45
 *  Author: Emaus
 */ 


#ifndef __MAIN_H__
#define __MAIN_H__

//#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Pong.hpp"

void initADC();
uint16_t readADC(uint8_t pin);
uint16_t randVal();

#endif /* __MAIN_H__ */