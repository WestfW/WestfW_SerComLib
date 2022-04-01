# Arduino\_sercomlib #

Many SAMD based Arduino boards have uncommitted SERCOM peripherals, or
SERCOMs that are theoretically aimed at a a particular purpose (I2C,
SPI) (but only if the appropriate library is also included by the user
sketch.)  This library permits these SERCOMs to be assigned a
different purpose by creating the appropriate Object (eg SerialN for a
UART) and ISR functions.

If a particular SERCOM is in use for another purpose (for example, you cannot include the Wire library AND redefine the I2C pins as a UART), you will get "multiply defined symbol" errors from the link stage of the build process.

## Basic Operation ##

There are several forms of the macros provided.  Which one to use depends on how much as already been defined in the board's variant.h file.

All of these require the SERCOM number, either a constant or a preprocessor definition that evaluates to a number, since the macros use "macro magic" to construct the proper ISR names (SERCOM1_Handler or similar.)  It is unfortunate that you can not provide a Sercom data structure.  This means that if you want to redefine the i2c pins as a UART, you need to look into the variant.h file to see which

### sercom\_MakeUart(sercomNumber, SerialNumber) ###
### sercom\_MakeUart_Pins(sercomNumber, SerialName rxPin, txPin) ###
### sercom\_MakeUart_Pads(sercomNumber,  SerialName, rxPin, txPin, rxPad, txPad) ###
### sercom\_MakeWire_Pins(sercomNumber, _i2cName, _sda, _scl) ###
