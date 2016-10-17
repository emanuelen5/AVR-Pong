/*
 * SSD1306_PONG.cpp
 *
 * Created: 2016-05-08 13:32:06
 * Author : Emaus
 */ 


/*
Connection diagram:

                        ------
(Reset)             PC6|01  28|PC5    (RPAD)
                    PD0|02  27|PC4    (LPAD)
                    PD1|03  26|PC3
                    PD2|04  25|PC2
                    PD3|05  24|PC1
                    PD4|06  23|PC0
                    Vcc|07  22|Gnd
                    Gnd|08  21|Aref
(RESET#)            PB6|09  20|AVcc
                    PB7|10  19|PB5    (SCK)
                    PD5|11  18|PB4
                    PD6|12  17|PB3    (MOSI)
                    PD7|13  16|PB2    (SS)      
                    PB0|14  15|PB1    (DATA/COMMAND#)
                        ------
*/

/*
Pin setup on OLED display with controller SSD1309
  _____                              ___________________________________________
 |     |                           |||||||||||||||||||||||||||||||||||||||||||||
 |     |---- SS                ----|CS||||||||||||||||||||||||||||||||||||||||||
 |     |---- DATA/COMMAND#     ----|DC||||||||||||||||||||||||||||||||||||||||||
 |  M  |---- RESET#            ----|RES|||||||||||||||||||||||||||||||||||||||||
 |  C  |---- MOSI              ----|D1|||||||||||||||||OLED|||||||||||||||||||||
 |  U  |---- SCK               ----|D0|||||||||||||||SSD 1306|||||||||||||||||||
 |     |---- VCC (5V)          ----|VCC|||||||||||||||||||||||||||||||||||||||||
 |     |---- GND (0V)          ----|GND|||||||||||||||||||||||||||||||||||||||||
 |     |                           |||||||||||||||||||||||||||||||||||||||||||||
  *****                              *******************************************
*/

#include "main.hpp"

#define REMOVE_REFRESH_INTERRUPT (TIMSK0 = 0)
#define SET_REFRESH_INTERRUPT (TIMSK0 = BV(OCIE0A))
Pong pong;

/** ISR on CTC for timer 0
**/
ISR(TIMER0_COMPA_vect) {
	cli();
	pong.stepBall();
	sei();
}

void initADC() {
	ADMUX = BV(REFS0); // AVcc, Left Adjusted Result
	ADCSRA |= BV(ADEN) | BV(ADSC); // Enable ADC, Start Conversion
	while (ADCSRA & BV(ADSC)); // Wait for conversion to complete before continuing
}

uint16_t readADC(uint8_t pin) {
	if (pin > 5 && pin != 8 && pin != 0xE && pin != 0xF) // Check that the pin corresponds to a pin which has an ADC connected to it
		return 0;
	ADMUX &= 0xF0; // Deselect current pin
	ADMUX |= pin; // Select pin C0-C5/other source
	ADCSRA |= BV(ADSC) | 7; // Start Conversion
	while (ADCSRA & BV(ADSC)); // Wait for conversion to finish
	return ADC;
}

uint16_t randVal() {
	return (readADC(PONG_L_PIN) ^ readADC(PONG_R_PIN));
}

void initRefreshInterrupt(void) {
	TCCR0A = BV(WGM01); // Clear on timer compare
	TCCR0B = BV(CS02); // CLK/256
	OCR0A = 65/SPEED_SCL; // 1 Mhz / 256 / 65 ~ 60 Hz
	SET_REFRESH_INTERRUPT; // Compare 0A interrupt
}

int main(void) {
	
	initADC();
	initRefreshInterrupt();
	
	pong.menu();
	
	sei();
	while (1) {
		pong.refreshPads(); // Draw position of pads from ADC values
		pong.refreshBall();
		// If someone has made a point, display menu and temporarily deactivate timer
		if (pong.madePoint(0) != 0) {
			REMOVE_REFRESH_INTERRUPT;
			pong.pointMenu();
			SET_REFRESH_INTERRUPT;
		}
	}
}