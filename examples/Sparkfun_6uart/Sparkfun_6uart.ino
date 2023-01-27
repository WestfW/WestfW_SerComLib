// #include <SAMD_pinmux_report.h>  //debuging support
#include "WestfW_SerComLib.h"
#include <wiring_private.h>

/*
 *    Jan 2023 by Bill Westfield.  Released to the public domain.
 *
 *   Attach the extra SERCOM peripherals to Serial Ports (UART mode.)
 *   Implements 6 hardware serial ports on a Sparkfun SAMD21 Breakout Pro
 *   One ("Serial1") on the normal Rx/Tx pins (0/1)
 *   One ("Serial2") on the pins normally used by I2C ("Wire")
 *   One ("Serial3") on the pins normally used by SPI (legacy ISP)
 *   one ("Serial4") on pins D11, D13
 *   (don't used Serial5, because it's an alias for Serial sometimes)
 *   one ("Serial6") on pins D3, D4 (but watch 2/4 pin-swap on some boards.)
 *   one (Serial) on pins 30/31 on the Sparkfun extra connector.
 *   This does mean that Wire and SPI libraries may NOT be used.
 *   Note that UART, I2C, and SPI all use SERCOM peripherals.
 *   That means that any SAMD that exposes I2C and SPI can use
 *    those pins for UART instead.
 */

/*
 * On Zero (or Adafruit/Sparkfun boards with same pinout):
 *  Sercom0 implements Serial1 using Pad2 and Pad3
 *  Sercom1 is exposed on pins D10..D13
 *  Sercom2 is exposed on pins D2..D5
 *  Sercom3 implements I2C using pad0 and 1
 *  Sercom4 implements SPI ("legacy ISP connector") using Pad0, 2, and 3.
 *  Sercom5 implements "Serial" on Zero (EDBG) or sparkfun (extra connector),
 *   but is dedicated to SPI2 for the flash on Adafruit Metro M0
 */
#define PIN_SERIAL2_RX SCL
#define PIN_SERIAL2_TX SDA

#define PIN_SERIAL3_RX MISO
#define PIN_SERIAL3_TX MOSI

#define PIN_SERIAL4_RX 13
#define PIN_SERIAL4_TX 11

#define PIN_SERIAL6_RX  3
#define PIN_SERIAL6_TX  4

#define Mon SERIAL_PORT_MONITOR

SERCOMLIB_MakeSerial_OnWIRE(3, 2) // Tx2 on SDA, Rx2 on SCL
SERCOMLIB_MakeSerial_OnSPI(4, 3)  // Tx3 on MISO, Rx3 on MISO, TX3 on MOSI

//                        SERCOM# Ser# rxpin txpin rxpad            txpad
#ifdef PIN_SERIAL4_RX
SERCOMLIB_MakeSerialPinsPads(1,   4,   13,   11,   SERCOM_RX_PAD_1, UART_TX_PAD_0)
#endif
#ifdef PIN_SERIAL6_RX
SERCOMLIB_MakeSerialPinsPads(2,   6,   3,    4,    SERCOM_RX_PAD_1, UART_TX_PAD_0)
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

char buffer[100];
const int monitor = false;

void setup() {
  // Start ports at different speeds to demo their independence.
  Mon.begin(9600);       // USB port

  Serial1.begin(9600);      // Serial1 (pins 0 and 1) alread defined.
  pullup(PIN_SERIAL1_RX);

  Serial2.begin(9600);      // I2C pins
  pullup(PIN_SERIAL2_RX);

  Serial3.begin(9600);      // SPI pins
  pullup(PIN_SERIAL3_RX);

#ifdef PIN_SERIAL4_RX
  Serial4.begin(9600);
  pinPeripheral(PIN_SERIAL4_RX, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);
  pullup(PIN_SERIAL4_RX);
#endif

#ifdef PIN_SERIAL6_RX
  Serial6.begin(9600);
  pinPeripheral(PIN_SERIAL6_RX, PIO_SERCOM_ALT);
  pinPeripheral(4, PIO_SERCOM_ALT);
  pullup(PIN_SERIAL6_RX);
#endif

  Serial.begin(9600);
  pullup(PIN_SERIAL_RX);

  delay(6000);          // Wait for USB Serial to finish initialized
                        //   But don't wait forever!
#ifdef SAMD_pinmux_h
  for (unsigned int pin = 0; pin <= PIN_SERIAL_RX; pin++) {  // For all defined pins
    pinmux_report(pin, buffer, 0);
    Mon.println(buffer);
  } // "pin" for loop
#endif
}

void loop() {
  if (monitor) {
    Mon.println("This is MONITOR");
  }
  Serial.println("This is Serial");    // Check Transmit
  while (Serial.available()) {          // Check receive
    Serial.print("Serial read ");
    Serial.println(Serial.read());
  }

  if (monitor) {
    Mon.println("1");
  }
  Serial1.println("This is Serial 1");    // Check Transmit
  while (Serial1.available()) {          // Check receive
    Serial1.print("Serial 1 read ");
    Serial1.println(Serial1.read());
  }

  if (monitor) {
    Mon.println("2");
  }
  Serial2.println("This is Serial 2");
  while (Serial2.available()) {
    Serial2.print("Serial 2 read ");
    Serial2.println(Serial2.read());
  }

  if (monitor) { Mon.println("3"); }
  Serial3.println("This is Serial 3");
  while (Serial3.available()) {
    Serial3.print("Serial 3 read ");
    Serial3.println(Serial3.read());
  }

  #ifdef PIN_SERIAL4_RX
  if (monitor) { Mon.println("4"); }
  Serial4.println("This is Serial 4");
  while (Serial4.available()) {
    Serial4.print("Serial 4 read ");
    Serial4.println(Serial4.read());
  }
  #endif

  #ifdef PIN_SERIAL6_RX
  if (monitor) { Mon.println("6"); }
  Serial6.println("This is Serial 6");
  while (Serial6.available()) {
    Serial6.print("Serial 6 read ");
    Serial6.println(Serial6.read());
  }
  #endif
  
  if (Mon.read() == 's') {  // "status report"
    show_pmux("");
    delay(10000);
  }
  delay(1000);  // wait a bit for more data.

}


void show_pmux(const char *title) {
#ifdef SAMD_pinmux_h
  static const int pins2show[] = {0, 1,   3, 4,   11, 13,   SDA, SCL,   MOSI, MISO, PIN_SERIAL_TX, PIN_SERIAL_RX};
  Mon.println(title);
  for (unsigned int pin = 0; pin < sizeof(pins2show)/sizeof(int); pin++) {  // For all defined pins
      pinmux_report(pins2show[pin], buffer, 0);
      Mon.println(buffer);
    } // "pin" for loop
    Mon.println();
#else
  (void)(title);
  Mon.println("No debug status available.");
#endif
}
