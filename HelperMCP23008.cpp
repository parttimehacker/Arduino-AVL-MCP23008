/* /////////////////////////////////////////////////////////////////////////////
Modern Art Electronics Project, MCP23008 Port Expander Class

Work based on the Adafruit library.

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

#include "HelperMCP23008.h"
#include <Wire.h>

// // // // //
// instantiate privates
HelperMCP23008::HelperMCP23008() {
    //Wire.begin(); // assume Wire.begin has been used in the main project
}

// // // // //
// init the chip
void HelperMCP23008::init( uint8_t i2cChannel ) {
    if (i2cChannel > 7) {
        i2cChannel = 7;
    }
    _i2cChannel = i2cChannel;

    _regWrite(MCP23008_IOCON,INTERRUPT_POLARITY_BIT);     // basic access
    //_regWrite(MCP23008_IOCON,SEQUENTIAL_ACCESS_BIT|INTERRUPT_POLARITY_BIT);     // register as input
        
    _regWrite(MCP23008_IODIR,PORT_DIR_IN);     // register as 8 bit input
    _register = 0;						// set to zero
    
    // initialize interrupt handling
    _regWrite(MCP23008_GPINTEN,0x00);   // interrupts disabled (1=interrupts enabled on change)
    _regWrite(MCP23008_IPOL,0x00);      // normal polarity (1=inverted polarity)
    _regWrite(MCP23008_DEFVAL,0x00);    // interupt on low (1=interrupts on high)
    _regWrite(MCP23008_INTCON,0x00);    // change from previous value (1=change from default)
}

// // // // //
// reset the chip
void HelperMCP23008::reset() {
    _regWrite(MCP23008_IODIR,0xFF);     // register as input
    _register = 0;						// set to zero
    
    // initialize interrupt handling
    _regWrite(MCP23008_GPINTEN,0x00);   // interrupts disabled (1=interrupts enabled on change)
    _regWrite(MCP23008_IPOL,0x00);      // normal polarity (1=inverted polarity)
    _regWrite(MCP23008_DEFVAL,0x00);    // interupt on low (1=interrupts on high)
    _regWrite(MCP23008_INTCON,0x00);    // change from previous value (1=change from default) 
}

// // // // // 
// define input and output ports on the MCP23008
void HelperMCP23008::setDirection(uint8_t direction) {
    _regWrite(MCP23008_IODIR,direction);     // 1-bit input; 0-bit output
}
	
// // // // //
// write one byte to the GPIO port
void HelperMCP23008::write(uint8_t byte) {
    _regWrite(MCP23008_GPIO,byte);
}

// // // // //
// read one byte from the GPIO port
uint8_t HelperMCP23008::read() {
    return _regRead(MCP23008_GPIO);
}

// // // // //
// configure inputs to pullups
void HelperMCP23008::configurePullUps(uint8_t offOrOn) {
    _regWrite(MCP23008_GPPU,offOrOn);
}

///////////////////////////////
// interrupt handling stuff
///////////////////////////////

// // // // //
// enable interrupts
void HelperMCP23008::interruptEnable(uint8_t interruptsEnabledMask) {
    _regWrite(MCP23008_GPINTEN,interruptsEnabledMask);
}

// // // // //
// set interrupt polarity
void HelperMCP23008::interruptPolarity(uint8_t polarity) {
    _regWrite(MCP23008_IPOL,polarity);
}

// // // // //
// set up interrupt values
void HelperMCP23008::defaultValue(uint8_t valuesToCompare) {
    _regWrite(MCP23008_DEFVAL,valuesToCompare);
}

// // // // //
// set up the interrupt bits
void HelperMCP23008::interruptControl(uint8_t interruptControlBits) {
     _regWrite(MCP23008_INTCON,interruptControlBits);
}

// // // // //
// get interupt register flags
uint8_t HelperMCP23008::getInterruptFlags() {
    return _regRead(MCP23008_INTF);
}

// // // // //
// get interupt register data
uint8_t HelperMCP23008::getInterruptData() {
    return _regRead(MCP23008_INTCAP);
}

// // // // //
// turn on a port the off after 50 milliseconds - a blink
void HelperMCP23008::blinkLED( uint8_t bit ) {
    if ((bit<0) || (bit>7)) {
        return;
    }
	_register |= bit;
	_regWrite(MCP23008_GPIO,_register);
	delay(50);
	_register &= ~bit;
	_regWrite(MCP23008_GPIO,_register);
}

// // // // //
// set bit mode - simulating A0-A5 on ATEMega328 firmware
void HelperMCP23008::pinMode( uint8_t bit, uint8_t value ) {
    if ((bit<0) || (bit>7)) {
        return;
    }
	uint8_t regData = _regRead(MCP23008_IODIR);
	if (value == INPUT) {
		regData  |= bit;
	} else {
		regData &= ~bit;
	}
	_regWrite(MCP23008_IODIR,regData);
}

// // // // //
// set a bit
void HelperMCP23008::digitalWrite( uint8_t bit, uint8_t value ) {
    if ((bit<0) || (bit>7)) {
        return;
    }
	_register = _regRead(MCP23008_GPIO);
	if (value == HIGH) {
		_register  |= bit;
	} else {
		_register &= ~bit;
	}
	_regWrite(MCP23008_GPIO,_register);
}

// // // // //
// read a bit
uint8_t HelperMCP23008::digitalRead( uint8_t bit ) {
    if ((bit<0) || (bit>7)) {
        return;
    }
	uint8_t regData = _regRead(MCP23008_GPIO);
	regData &= bit;
	return regData;
}

///////////////////////////////
// private reads and writes
///////////////////////////////

// // // // //
// private write to a register
void HelperMCP23008::_regWrite(uint8_t reg, uint8_t byte) {    
    // Write to specified MCP23008 register
    Wire.beginTransmission(MCP23008_ADDRESS | _i2cChannel);
    Wire.write (reg);
    Wire.write (byte);
    Wire.endTransmission ();
}

// // // // //
// internal read
uint8_t HelperMCP23008::_regRead(uint8_t reg) {                
    // Read from specified MCP23008 register
    uint8_t byte = 0x00;
    Wire.beginTransmission(MCP23008_ADDRESS | _i2cChannel);
    Wire.write(reg);
    Wire.endTransmission();
    
    Wire.requestFrom(MCP23008_ADDRESS | _i2cChannel, 1);
    while(Wire.available()) {
        byte = Wire.read();
    }
    return byte;
}