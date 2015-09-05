/* /////////////////////////////////////////////////////////////////////////////
Modern Art Electronics Project, MCP23008 Port Expander Class

Work based in part on the Adafruit library.

Copyright (c) 2014. cyWren Systems, Inc., MIT License.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
////////////////////////////////////////////////////////////////////////// */

#ifndef _HelperMCP23008_H
#define _HelperMCP23008_H

#include "Arduino.h"

#define MCP23008_ADDRESS 	0x20

// registers

#define MCP23008_IODIR		0x00
#define MCP23008_IPOL 		0x01
#define MCP23008_GPINTEN 	0x02
#define MCP23008_DEFVAL 	0x03
#define MCP23008_INTCON 	0x04
#define MCP23008_IOCON 		0x05
#define MCP23008_GPPU 		0x06
#define MCP23008_INTF 		0x07
#define MCP23008_INTCAP 	0x08
#define MCP23008_GPIO 		0x09
#define MCP23008_OLAT 		0x0A

// constants

#define INTERRUPT_POLARITY_BIT 	0x02
#define SEQUENTIAL_ACCESS_BIT   0x20
#define PORT_DIR_OUT   			0x00
#define PORT_DIR_IN    			0xFF

class HelperMCP23008 {
public:
    HelperMCP23008();
    
    // initialize the device with defaults. up to eight devices can be defined
    void init( uint8_t i2cChannel );
    void reset();
    
    // iocon address commands including in/out, pull ups and byte addressing
	void setDirection(uint8_t direction);
	void configurePullUps(uint8_t offOrOn);
        
    // byte oriented read and write to the gpio port
    uint8_t read();
    void write(uint8_t byte);
    
    // get information from the last interrupts and those pending
    uint8_t getInterruptFlags();
    uint8_t getInterruptData();
    
    // define the interrupt processing for each pin on the GPIO input only
    void interruptPolarity(uint8_t polarity);
    void interruptEnable(uint8_t interruptsEnabledMask);
    void defaultValue(uint8_t valuesToCompare);
    void interruptControl(uint8_t interruptControlBits);
    
    // simulate a single bit on the GPIO connected to an LED
    void blinkLED( uint8_t bit );
    
    // simulate a single bit on the GPIO - in or out per pin 
    void pinMode( uint8_t bit, uint8_t value );
    void digitalWrite( uint8_t bit, uint8_t value );
    uint8_t digitalRead( uint8_t bit );
           
private:
    uint8_t _i2cChannel;
    uint8_t	_register;

    void _regWrite(uint8_t reg, uint8_t byte);   // Write to specified MCP23008 register
    uint8_t _regRead(uint8_t reg);               // Read from specified MCP23008 register
};

#endif // _HelperMCP23008_H