#include "WestfW_SerComLib.h"
/*
 *    Jan 2023 by Bill Westfield.  Released to the public domain.
 *
 *   Attach the extra SERCOM peripherals to Serial Ports (UART mode.)
 *   Implements a total of 3 hardware serial ports on the Adafruit QTPy
 *   One ("Serial1") on the normal Rx/Tx pins.
 *   One ("Serial2") on the pins normally used by I2C ("Wire")
 *   One ("Serial3") on the pins normally used by SPI
 *   This does mean that Wire and SPI libraries may NOT be used.
 *   Note that UART, I2C, and SPI all uses SERCOM peripherals.
 *   That means that any SAMD that exposes I2C and SPI can use
 *    those pins for UART instead.
 */

/*
 *-----------
 * A0     5V
 * A1    GND 
 * A2   3.3V
 * A3    TX3
 * TX2   RX3
 * RX2     8
 * TX1   RX1
 *-----------
 * on a QTPy:
 *  Serial1 uses Sercom0, Pad2 and Pad3
 *  I2C exposes Sercom1 or Sercom3, Pad0, and Pad1  (use Sercom1)
 *  SPI exposes Sercom0 or Sercom2, Pad1, 2, and 3. (we must use Sercom2)
 *  The on-board flash chip (if used) is on Sercom3. (painful to use.)
 *  Sercom4 does not appear on any connected pins.
 *  Sercom5 Pad2,3 end up being used for USB.
 *
 * on a Seeed Xiao:
 *  Serial1 uses Sercom4, Pad0 and Pad1
 *  I2C exposes Sercom0 or Sercom2, Pad0, and Pad1
 *  SPI exposes Sercom0, Pad1, 2, and 3.
 *
 * On Zero (or Adafruit/Sparkfun boards with same pinout):
 *  Serial1 uses Sercom0, Pad2 and Pad3
 *  The on-board flash chip is on Sercom3.
 *  I2C exposes Sercom1 or Sercom3 or Sercom5 Pad0, and Pad1
 *  SPI exposes Sercom0 or Sercom4, Pad0, 2, and 3.
 *  
 */

#define PIN_SERIAL2_RX 5
#define PIN_SERIAL2_TX 4
#define PIN_SERIAL3_RX 9
#define PIN_SERIAL3_TX 10

#if defined(ADAFRUIT_QTPY_M0)
//                         SERCOM#, Serial#, rxpin txpin  rxpad            txpad
SERCOMLIB_MakeSerialPinsPads(  1,      2,       5,  4,    SERCOM_RX_PAD_1, UART_TX_PAD_0)
SERCOMLIB_MakeSerialPinsPads(  2,      3,       9,  10,   SERCOM_RX_PAD_1, UART_TX_PAD_2)
#elif defined ADAFRUIT_ 
#else
SERCOMLIB_MakeSerial_OnWIRE(1, 2)
SERCOMLIB_MakeSerial_OnSPI(2, 3)
#endif
/*
 * turning on a pullup (even though the pin is set to use UART)
 * prevents line noise from looking like characters.
 */
void pullup (int pin) {
  PortGroup *port = digitalPinToPort(pin);
  port->OUTSET.reg = g_APinDescription[pin].ulPin ;
  port->PINCFG[g_APinDescription[pin].ulPin].reg |= PORT_PINCFG_PULLEN ;
}

void setup() {
  // Start ports at different speeds to demo their independence.
  Serial.begin(9600);     // USB port

  Serial1.begin(9600);     // Serial1 (pins 0 and 1) alread defined.
  pullup(PIN_SERIAL1_RX);

  Serial2.begin(9600);    // I2C pins
  pullup(PIN_SERIAL2_RX);

  Serial3.begin(9600);    // SPI pins
  pullup(PIN_SERIAL3_RX);
  
  while (!Serial)           // Wait for USB Serial to finish initialized
    ;
}

void loop() {
  Serial.println("This is Serial");
  Serial.println("1");
  Serial1.println("This is Serial 1");    // Check Transmit
  while (Serial1.available()) {          // Check receive
    Serial1.print("Serial 1 read ");
    Serial1.println(Serial1.read());
  }
  Serial.println("2");
  Serial2.println("This is Serial 2");
  while (Serial2.available()) {
    Serial2.print("Serial 2 read ");
    Serial2.println(Serial2.read());
  }
  Serial.println("3");
  Serial3.println("This is Serial 3");
  while (Serial3.available()) {
    Serial3.print("Serial 3 read ");
    Serial3.println(Serial3.read());
  }
 
  delay(1000);  // wait a bit for more data.

}
