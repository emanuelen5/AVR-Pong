#ifndef __SSD1306_H__
#define __SSD1306_H__

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "5x8_font.hpp"

/** SSD1306 Controller Driver
  *
  */
class SSD1306 {
public:
    /** Construct a new SSD1306 object.*/
    SSD1306();
 
    // ----------------------------------- HARDWARE CONTROL -----------------------------------
 
    /** Initialise the display with defaults.*/
    void initialise();

    /** Send reset to display. Sets all configuration to default. */
    void reset();
 
    /** Turn the whole display on/off. Keeps all GDDRAM intact. 
    *   @param b TRUE = turn on display, or FALSE = turn off display
    **/
    void power(uint8_t b);

    /** Turn on all OLEDS/show GDDRAM content.
    *   @param b TRUE = display test, or FALSE = display GDDRAM content
    */
    void set_display_test(uint8_t b);
 
    /** Set the display contrast.
     *  @param contrast The contrast, from TRUE to 256.
     */
    void set_contrast(uint8_t contrast) ;
    
    /** Set the display to normal or inverse.
     *  @param b FALSE = normal mode, or TRUE = inverse mode.
     */
    void set_inverse(uint8_t b);
    
    /** Set the display start line.  This is the line at which the display will start rendering.
     *  @param line A value from 0 to 63 denoting the line to start at.
     */
    void set_display_start_line(uint8_t line);
    
    /** Set the segment remap state.  This allows the module to be addressed as if flipped horizontally.
      * NOTE: Changing this setting has no effect on data already in the module's GDDRAM.
      * @param b FALSE = column address 0 <=> segment 0 (the default), TRUE = column address 127 <=> segment 0 (flipped).
      */
    void set_segment_remap(uint8_t b);
    
    /** Set the vertical shift by COM.
      * @param offset The number of rows to shift, from 0 - 63.
      */
    void set_display_offset(uint8_t offset);
    
    /** Set the multiplex ratio.
     *  @param ratio MUX will be set to (ratio+1). Valid values range from 15 to 63 - MUX 16 to 64.
     */
    void set_multiplex_ratio(uint8_t ratio);
    
    /** Set COM output scan direction.  If the display is active, this will immediately vertically
      * flip the display.
      * @param b FALSE = Scan from COM0 (default), TRUE = reversed (scan from COM[N-1]).
      */
    void set_com_output_scan_direction(uint8_t b);
    
    /** Set COM pins hardware configuration.
      * @param sequential FALSE = Sequental COM pin configuration, TRUE = Alternative COM pin configuration (default).
      * @param left_right_remap FALSE = Disable COM left/right remap (default), TRUE = enable COM left/right remap.
      */
    void set_com_pins_hardware_configuration(uint8_t sequential, uint8_t left_right_remap);
    
    // -------------------------------------------------- ADDRESSING --------------------------------------------------
    
    /** Set memory addressing mode to the given value.
      * @param mode FALSE = Horizontal addressing mode, TRUE = Vertical addressing mode, or 2 = Page addressing mode (PAM).  2 is the default.
      */
    void set_memory_addressing_mode(uint8_t mode);
    
    /** Page Addressing Mode: Set the column start address register for
      * page addressing mode.
      * @param address The address (full byte).
      */
    void pam_set_start_address(uint8_t address);    
    
    /** Set the GDDRAM page start address for page addressing mode.
      * @param address The start page, 0 - 7.
      */
    void pam_set_page_start(uint8_t address);
    
    /** Set page start and end address for horizontal/vertical addressing mode.
      * @param start The start page, 0 - 7.
      * @param end The end page, 0 - 7.
      */
    void hv_set_page_address(uint8_t start, uint8_t end);
    
    /** Set column address range for horizontal/vertical addressing mode.
      * @param start Column start address, 0 - 127.
      * @param end Column end address, 0 - 127.
      */
    void hv_set_column_address(uint8_t start, uint8_t end);
    
    // ----- TIMING & DRIVING -----
    /** Set the display clock divide ratio and the oscillator frequency.
      * @param ratio The divide ratio, default is 0.
      * @param frequency The oscillator frequency, 0 - 127. Default is 8.  
      */
    void set_display_clock_ratio_and_oscillator_frequency(uint8_t ratio, uint8_t frequency);
    
    /** Set the precharge period.
      * @param phase1 Phase 1 period in DCLK clocks.  1 - 15, default is 2.
      * @param phase2 Phase 2 period in DCLK clocks.  1 - 15, default is 2.
      */
    void set_precharge_period(uint8_t phase1, uint8_t phase2);
    
    /** Set the Vcomh deselect level.
      * @param level 0 = 0.65 x Vcc, 1 = 0.77 x Vcc (default), 2 = 0.83 x Vcc.
      */
    void set_vcomh_deselect_level(uint8_t level);
    
    /** Perform a "no operation".
      */
    void nop();
    
    /** Enable/disable charge pump.
      @param enable FALSE to disable, TRUE to enable the internal charge pump.
      */
    void set_charge_pump_enable(uint8_t enable);
    
    // -------------------------------------------- BUFFER EDITING --------------------------------------------
	
	/** Clears the whole memory buffer
	**/
	void clear();
 
	/** Sets a pixel in the frame buffer. Display must be refreshed to display the change.
	 @param x X-position
	 @param y Y-position
	*/
    void set_pixel(uint8_t x, uint8_t y);
	
	/** Clears a pixel in the frame buffer. Display must be refreshed to display the change.
	 @param x X-position
	 @param y Y-position
	*/
    void clear_pixel(uint8_t x, uint8_t y);
	
	/** Toggles a pixel in the frame buffer. Display must be refreshed to display the change.
	 @param x X-position
	 @param y Y-position
	*/
	void toggle_pixel(uint8_t x, uint8_t y);
	
	/** Draws a line in the frame buffer. Display must be refreshed to display the change.
	 @param x0 Line starting X-position
	 @param y0 Line starting Y-position
	 @param x1 Line ending X-position
	 @param y1 Line ending Y-position
	*/
	void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t action);
	
	/** Draws a vertical line (or pattern) in the frame buffer. Display must be refreshed to display the change.
	 @param x X-position
	 @param b Byte for each vertical block of 8-pixels
	*/
	void vLine(uint8_t x, uint8_t b);
	
	/** Draws a byte with start in an x-y position. Display must be refreshed to display the change.
	 @param x X-position
	 @param y Y-position
	 @param val Byte to put into the position
	*/
	void set_block(uint8_t x, uint8_t y, uint8_t val);
	
	/** Writes a character to a specific location on the screen buffer
	 @param c Character to write (ASCII encoded)
	 @param x X-start position
	 @param y Y-start position
	*/
	void writeChar(const char c, uint8_t x, uint8_t y);
	
	/** Writes a string to a specific location on the screen buffer
	 @param c Pointer to character array to write (ASCII encoded)
	 @param x X-start position
	 @param y Y-start position
	*/
	void writeStr(const char* c, uint8_t x, uint8_t y);
	
	/** Refreshes the whole display
	*/
	void refresh();
	
	/** Refreshes only the page corresponding to the pixel
	*/
	void refresh(uint8_t x, uint8_t y);
	
	/** Refreshes part of a display between pixel {x0,y0} and {x1,y1} including the other pixels which correspond to the same "pages".
	 @param x0 Starting X-position
	 @param y0 Starting Y-position
	 @param x1 Ending X-position
	 @param y1 Ending Y-position
	*/
	void refresh(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
 
private:
    uint8_t _screen[1024];
 
    void initSPI(void);
    void SPI_send(const uint8_t DATA);

    void _command(const uint8_t cmd);
    void _data(const uint8_t value);
};
 
#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64

// PINs for SSD/SPI
#define DDR_SSD  DDRB
#define PORT_SPI PORTB
#define PORT_SSD PORT_SPI
#define DD_RES   PB6   // Reset pin
#define DD_SCK   PB5   // Slave clock
#define DD_MISO  PB4   // Master in, slave out
#define DD_MOSI  PB3   // Master out, slave in
#define DD_SS    PB2   // Slave select
#define DD_DC    PB1   // Data/Command#

#define SSD_COMMAND                PORT_SSD &= ~BV(DD_DC)
#define SSD_DATA                   PORT_SSD |= BV(DD_DC)
#define SPI_SLAVE_SELECT           PORT_SPI &= ~BV(DD_SS)
#define SPI_SLAVE_DESELECT         PORT_SPI |= BV(DD_SS)

#define TRUE        1
#define FALSE       0
#define BV(BIT)     (1<<(BIT))
#define abs(a)      ((a)<0?-(a):(a))
 
#endif