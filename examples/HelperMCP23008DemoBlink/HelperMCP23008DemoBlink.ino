
/* /////////////////////////////////////////////////////////////////////////////
parttimehacker@gmail.com; cyWren Systems Inc; 

MIT License.
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

#include <Wire.h>
#include <HelperMCP23008.h>
#include "Arduino.h"

///////
// Arduino ATMega328 Pins
///////

#define D2_INT0_PIN         2	// Digital		Interrupt 0
#define D3_INT1_PWM_PIN     3	// Digital	PWM	Interrupt 1
#define D4_PIN              4	// Digital
#define D5_PWM_PIN          5	// Digital      PWM
#define D6_PWM_PIN          6	// Digital	PWM
#define D7_PIN              7	// Digital
#define D8_PIN              8	// Digital
#define D9_PWM_PIN	    9	// Digital	PWM
#define D10_PWM_PIN	   10	// Digital	PWM
#define D11_PWM_MOSI_PIN   11	// Digital	PWM	SPI	MOSI
#define D12_PWM_MISO_PIN   12	// Digital		SPI	MISO
#define D13_PWM_SCK_PIN    13	// Digital		SPI	SCK

#define	A0_PIN            A0	// Analog
#define	A1_PIN	          A1	// Analog
#define	A2_PIN            A2	// Analog
#define	A3_PIN            A3	// Analog
#define	A4_SDA_PIN	  A4	// Analog		I2C	SDA
#define	A5_SCL_PIN	  A5	// Analog		I2C	SCL

///////
// pin assignments

////////////////////////////////////////////////////////////////////////////
// Define MCP23008 PINS
////////////////////////////////////////////////////////////////////////////

// GPIO ports

#define MCP08_0_BIT   0x01
#define MCP08_1_BIT   0x02
#define MCP08_2_BIT   0x04
#define MCP08_3_BIT   0x08
#define MCP08_4_BIT   0x10
#define MCP08_5_BIT   0x20
#define MCP08_6_BIT   0x40
#define MCP08_7_BIT   0x80

///////
// port assignments

#define GREEN_LED    MCP08_0_BIT
#define RED_LED      MCP08_1_BIT


////////////////////////////////////////////////////////////////////////////
// Object instantiations
///////

HelperMCP23008 mcp;  // 8-bit port expander with interrupt capability

bool ledOn = false;

////////////////////////////////////////////////////////////////////////////
// generic methods
///////

void selfTest() {
  Serial.println("Completed Self Test");
}


/////////////////////////////////////////////////////////////////////////////
// Interrupt Handlers
///////


////////////////////////////////////////////////////////////////////////////
// I2C stuff 
///////


/////////////////////////////////////////////////////////////////////////////
// Initialization
///////

void setup() {
  
  // analogReference(EXTERNAL); // important if required based on your project

  Serial.begin(9600);
  while(!Serial);
  Serial.println("\n\rHelperMCP23008 Demo");
  
  // initialize the MCP23008
  mcp.init(0);                        // this is address zero of a possible eight MCP23008 addresses
  mcp.setDirection(PORT_DIR_OUT);     // set all ports to output mode
  mcp.write(0x00);                    // initalize ports to zero
  
  mcp.pinMode(RED_LED,PORT_DIR_OUT);
  mcp.digitalWrite(RED_LED,HIGH);
  
  // simple status light show
  selfTest();
  
  // get ready, get set, go!
  delay(500);
  Serial.println("Looping ...");
  
}


////////////////////////////////////////////////////////////////////////////
// Main control logic
///////

void loop() {
  
  delay(1000);
  
  if (ledOn) {
    
    ledOn = false;
    
    // register write
    mcp.write(0x00);
    
    // simulated single bit write
    mcp.digitalWrite(RED_LED,LOW);
    
  } else {
    
    ledOn = true;
  
    mcp.write(GREEN_LED);
    mcp.digitalWrite(RED_LED,HIGH);
  }
  
}

