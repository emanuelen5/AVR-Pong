/*
 * SSD1306.c
 * 
 * The image loaded is Alice on a crayfish party.
 *
 * Created: 2016-05-08 10:53
 * Author : Emaus
 */ 


/*
Pin setup on OLED display with controller SSD1309
  _____                             ___________________________________________
 |     |                           |||||||||||||||||||||||||||||||||||||||||||||
 |     |---- SS                ----|CS||||||||||||||||||||||||||||||||||||||||||
 |     |---- DATA/COMMAND#     ----|DC||||||||||||||||||||||||||||||||||||||||||
 |  M  |---- RESET#            ----|RES|||||||||||||||||||||||||||||||||||||||||
 |  C  |---- MOSI              ----|D1|||||||||||||||||OLED|||||||||||||||||||||
 |  U  |---- SCK               ----|D0|||||||||||||||SSD 1306|||||||||||||||||||
 |     |---- VCC (5V)          ----|VCC|||||||||||||||||||||||||||||||||||||||||
 |     |---- GND (0V)          ----|GND|||||||||||||||||||||||||||||||||||||||||
 |     |                           |||||||||||||||||||||||||||||||||||||||||||||
  *****                             *******************************************
*/

#include "ssd1306.hpp"
 
SSD1306::SSD1306() {
	clear();
	initialise();
}

void SSD1306::clear() {
	memset(_screen, 0, sizeof(uint8_t)*SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8);
}
 
void SSD1306::power(uint8_t b) {
    _command(b?0xAF:0xAE);
}

void SSD1306::reset() {
    // Start by reset
    PORT_SSD |= BV(DD_RES);
    PORT_SSD &= ~BV(DD_RES);
    _delay_us(4); // Must be reset for at least 3us before use
    PORT_SSD |= BV(DD_RES);
}
 
void SSD1306::set_inverse(uint8_t b) {
    _command(b ? 0xA7 : 0xA6);
}
 
void SSD1306::set_display_offset(uint8_t offset) {
    _command(0xD3);
    _command(offset & 0x3F); 
}
 
void SSD1306::set_contrast(uint8_t contrast)  {
    _command(0x81);
    _command(contrast);
}
 
void SSD1306::set_display_start_line(uint8_t line) {
    _command(0x40 | line);
}
 
void SSD1306::set_segment_remap(uint8_t b) {
    _command(b ? 0xA1 : 0xA0);
}
 
void SSD1306::set_multiplex_ratio(uint8_t ratio) {
    _command(0xA8);
    _command(ratio & 0x3F);
}
 
void SSD1306::set_com_output_scan_direction(uint8_t b) {
    _command(b ? 0xC8 : 0xC0);
}
 
void SSD1306::set_com_pins_hardware_configuration(uint8_t sequential, uint8_t left_right_remap) {
    _command(0xDA);
    _command(0x02 | ((sequential & 1) << 4) | ((left_right_remap & 1) << 5));
}

void SSD1306::pam_set_start_address(uint8_t address) {
    // "Set Lower Column Start Address for Page Addressing Mode"
    _command(address & 0x0F);
    
    // "Set Higher Column Start Address for Page Addressing Mode"
    _command(0x10 | (address>>4));
}
 
void SSD1306::set_memory_addressing_mode(uint8_t mode) {
    _command(0x20);
    _command(mode & 0x3);
}
 
void SSD1306::hv_set_column_address(uint8_t start, uint8_t end) {
    _command(0x21);
    _command(start & 0x7F);
    _command(end & 0x7F);
}
 
void SSD1306::hv_set_page_address(uint8_t start, uint8_t end) {
    _command(0x22);
    _command(start & 0x07);
    _command(end & 0x07);
}
 
void SSD1306::pam_set_page_start(uint8_t address) {
    _command(0xB0 | (address & 0x07));
}
 
void SSD1306::set_display_clock_ratio_and_oscillator_frequency(uint8_t ratio, uint8_t frequency) {
    _command(0xD5);
    _command((ratio & 0x0F) | (frequency << 4));
}
 
void SSD1306::set_precharge_period(uint8_t phase1, uint8_t phase2) {
    _command(0xD9);
    _command((phase1 & 0x0F) | (phase2 << 4));
}
 
void SSD1306::set_vcomh_deselect_level(uint8_t level) {
    _command(0xDB);
    _command((level & 0x03) << 4);
}
 
void SSD1306::nop() {
    _command(0xE3);
}
 
void SSD1306::set_charge_pump_enable(uint8_t enable) {
    _command(0x8D);
    _command(enable ? 0x14 : 0x10);
}

void SSD1306::set_display_test(uint8_t b) {
    _command(b?0xA5:0xA4);
}
 
void SSD1306::initialise() {
	initSPI();
	
    reset();
    power(FALSE);
    
    set_multiplex_ratio(0x3F); // 1/64 duty
    set_display_offset(0);
    set_display_start_line(0);
    set_segment_remap(TRUE);
    set_com_output_scan_direction(TRUE);
    set_precharge_period(0x1, 0xF);
    set_com_pins_hardware_configuration(1, 0);
    set_contrast(0x7F);
    set_memory_addressing_mode(0); // horizontal addressing mode; across then down
    set_display_test(FALSE);
    set_inverse(FALSE);
    set_display_clock_ratio_and_oscillator_frequency(0x0, 0xF);
    set_charge_pump_enable(TRUE);
	
    power(TRUE);
    
    pam_set_start_address(0);
    pam_set_page_start(0);
	refresh();
}

/************************************************************************/
/* Initializes SPI in master mode with clock/2                          */
/************************************************************************/
void SSD1306::initSPI(void) {
    PORT_SSD |= BV(DD_MISO) | BV(DD_RES);
    DDR_SSD |= BV(DD_MOSI) | BV(DD_SCK) | BV(DD_SS) | BV(DD_DC) | BV(DD_RES); // Enable output on MISO, SCK, SS, DC and RES pins
    SPCR |= BV(SPE) | BV(MSTR); // Enable SPI | Master device | Clock/4
    SPCR |= BV(CPOL) | BV(CPHA); // Mode 3: Setup on falling, sample on rising
    SPSR |= BV(SPI2X); // Double clock speed => Clock/2
}

/***************************************************************************/
/* Sends data with SPI. Blocking until previous data transfer is complete. */
/***************************************************************************/
void SSD1306::SPI_send(const uint8_t DATA) {
    SPDR = DATA;
    while (!(SPSR & BV(SPIF)));
}

void SSD1306::_command(const uint8_t cmd) {
    SSD_COMMAND; // Command
    SPI_SLAVE_SELECT;
    SPI_send(cmd);
    SPI_SLAVE_DESELECT;
}

void SSD1306::_data(const uint8_t value) {
    SSD_DATA; // Data
    SPI_SLAVE_SELECT;
    SPI_send(value);
    SPI_SLAVE_DESELECT;
}

void SSD1306::set_pixel(uint8_t x, uint8_t y) {
	_screen[x + y/8*SSD1306_LCDWIDTH] |= BV(y%8);
}

void SSD1306::clear_pixel(uint8_t x, uint8_t y) {
	_screen[x + y/8*SSD1306_LCDWIDTH] &= ~BV(y%8);
}

void SSD1306::toggle_pixel(uint8_t x, uint8_t y) {
	_screen[x + y/8*SSD1306_LCDWIDTH] ^= BV(y%8);
}

void SSD1306::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t action) {
	int steep = abs(y1 - y0) > abs(x1 - x0);
	int t;
	
	if (steep) {
		t = x0; x0 = y0; y0 = t;
		t = x1; x1 = y1; y1 = t;
	}
	
	if (x0 > x1) {
		t = x0; x0 = x1; x1 = t;
		t = y0; y0 = y1; y1 = t;
	}
	
	int dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);
	
	int err = dx / 2;
	int ystep;
	
	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}
	
	for (; x0<x1; x0++) {
		if (steep) {
			switch (action)
			{
				case 0:
				set_pixel(y0, x0);
				break;
				case 1:
				clear_pixel(y0, x0);
				break;
				default:
				toggle_pixel(y0,x0);
			}
		} else {
			switch (action)
			{
				case 0:
				set_pixel(x0, y0);
				break;
				case 1:
				clear_pixel(x0, y0);
				break;
				default:
				toggle_pixel(x0,y0);
			}
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void SSD1306::vLine(uint8_t x, uint8_t b) {
	uint8_t y;
	for (y = 0; y<8; y++)
		_screen[x+y*SSD1306_LCDWIDTH] = b;
}

void SSD1306::set_block(uint8_t x, uint8_t y, uint8_t val) {
	uint16_t p1 = x + (uint16_t)y / 8 * SSD1306_LCDWIDTH;
	// Not even multiple of 8
	if (y%8) {
		uint16_t p2 = x + (uint16_t)(y / 8 + 1) * SSD1306_LCDWIDTH;
		// Set lower part of byte
		if (y < 64) {
			_screen[p1] |= val<<(y%8);
			_screen[p1] &= (val<<(y%8)) | (0xFF>>(8-y%8));
		}
		
		// Set upper part of byte
		if (y < 64-7 || y > 249) {
			_screen[p2] |= val>>(8-y%8);
			_screen[p2] &= (val>>(8-y%8)) | (0xFF<<(y%8));
		}
	} else {
		_screen[p1] = val;
	}
}

void SSD1306::writeChar(const char c, uint8_t x, uint8_t y) {
	uint8_t block;
	for (uint8_t i = 0; i<5; i++) {
		block = font(c, i);
		set_block(x++, y, block);
	}
}

void SSD1306::writeStr(const char* c, uint8_t x, uint8_t y) {
	uint8_t i = 0;
	while (c[i]) {
		writeChar(c[i++], x, y);
		x += 5;
	}
}

void SSD1306::refresh() {
	uint16_t i;
	// Set range to whole display
	hv_set_column_address(0, 127);
	hv_set_page_address(0, 7);
	
	SSD_DATA; // Data
	SPI_SLAVE_SELECT;
	for (i=0; i<sizeof(_screen); i++) {
		SPI_send(_screen[i]);
	}
	SPI_SLAVE_DESELECT;
}

void SSD1306::refresh(uint8_t x, uint8_t y) {
	x %= 128; // Maximum = 127
	
	uint8_t row;
	// Refresh whole rows which correspond to the pixels
	row = y/8; // [0,63]  => [0,7]
	row %= 8; // Maximum = 7
	
	hv_set_column_address(x, x);
	hv_set_page_address(row, row);
	
	SSD_DATA;
	SPI_SLAVE_SELECT;
	SPI_send(_screen[x+row*SSD1306_LCDWIDTH]);
	SPI_SLAVE_DESELECT;
}

void SSD1306::refresh(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	uint8_t t;
	if (x0 > x1) {
		t = x1;
		x1 = x0;
		x0 = t;
	}
	if (y0 > y1) {
		t = y1;
		y1 = y0;
		y0 = t;
	}

	x0 %= 128; // Maximum = 127
	x1 %= 128;
	
	uint8_t row0, row1;
	// Refresh whole rows which correspond to the pixels
	row0 = y0/8; // [0,63]  => [0,7]
	row1 = y1/8;
	row0 %= 8; // Maximum = 7
	row1 %= 8;
	
	
	hv_set_column_address(x0, x1);
	hv_set_page_address(row0, row1);
	
	SSD_DATA;
	SPI_SLAVE_SELECT;
	uint8_t x,row;
	for (row=row0; row<=row1; row++) {
		for (x=x0; x<=x1; x++) {
			SPI_send(_screen[x+row*SSD1306_LCDWIDTH]);
		}
	}
	SPI_SLAVE_DESELECT;
}