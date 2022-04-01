# Arduin_sercomlib #

Many SAMD based Arduino boards have uncommitted SERCOM peripherals, or
SERCOMs that are theoretically aimed at a a particular purpose (I2C,
SPI) (but only if the appropriate library is also included by the user
sketch.)  This library permits these SERCOMs to be assigned a
different purpose by creating the appropriate Object (eg SerialN for a
UART) and ISR functions.

If a particular SERCOM is in use for another purpose (for example, you cannot include the Wire library AND redefine the I2C pins as a UART), you will get "multiply defined symbol" errors from the link stage of the build process.

## Basic Operation ##
There are several forms of the macros provided.  Which one to use depends on how much as already been defined in the board's variant.h file.
sercom_MakeUart(int _sercomNum, int _serNumber)
sercom_MakeUart_Pins(_sercomNum, _serName, _rx, _tx)
#define sercom_MakeUart_Pads(_sercomNum, _serName, _rx, _tx, _rxpad, _txpad)
#define sercom_MakeWire_Pins(_sercomNum, _i2cName, _sda, _scl)
