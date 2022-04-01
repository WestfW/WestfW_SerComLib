#include <sercomlib.h>
/*
 *    April 2021 by Bill Westfield.  Released to the public domain.
 *
 *   Attach extra SERCOM peripherals to Serial Ports (UART mode.)
 *   This sketch demonstrates re-purposing the I2C and SPI pins of an
 *   Adafruit QT Py M0 board as UARTs, for a total of 3 UARTS
 *   Serial1: marked as tx(d6)/rx(d7) on board, sercom0
 *   SerialI2C: Marked as SDA(tx, D11)/SCL(rx, d5) on board, sercom1
 *   SerialSPI: Marked as MISO, sercom2
 *   There is also sercom3, used for SPI access to the optional SPI flash chip.
 *   sercom3 and sercom5 are not availble because their pins do other things.
 */

sercom_MakeUart_Pins(1, SerialI2C, SCL, SDA)  // SerialI2C - I2C pins

void setup() {
  Serial.begin(9600);
  SerialI2C.begin(9600);

  while (!Serial)           // Wait for USB Serial to finish initializing
    ;
}

void loop() {
  Serial.println("This is Serial");

  SerialI2C.println("This a Serial I2C");
  while (SerialI2C.available()) {
    SerialI2C.print("Serial I2C read ");
    SerialI2C.println(SerialI2C.read());
  }
  delay(1000);
}
