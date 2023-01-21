/*
 * sercomlib.h
 * July 2020 - Jan 2023, by Bill Westfield
 * Released to the public domain.
 *
 * This defines macros to pair up "un-used" sercom interfaces with
 *  Arduino driver-level definitions like UART/HardwareSerial, on
 *  SAMD51-based boards.
 *
 *  Use it like:
 *     sercom_MakeSerial(sercomNumber, SerialNumber)
 *  It's all relatively ugly pre-processor magic, and it
 *  assumes that symbols like sercomN, (from variant.cpp)
 *  and PIN_SERIALn_TX, PIN_SERIALn_TX (from variant.h)
 *  are already defined (True for Adafruit's Grand Central Station board,
 *  but not for some others.
 *  SerialN and the SERCOMn_x_Handler() end up being defined.
 *
 *  At some point, this might get expanded to allow I2C and SPI as well,
 *  and support other chip families.
 */

#include <variant.h>

#if SAMD51_SERIES || defined(_SAMD51_)
// Use this form where there are pre-defined PIN_SERIALn_RX and
//   PIN_SERIALn_TX symbols
#define SERCOMLIB_MakeSerial(_sercom, _serialNum) \
  Uart Serial##_serialNum( &sercom##_sercom, PIN_SERIAL##_serialNum##_RX, \
                           PIN_SERIAL##_serialNum##_TX,                   \
                           SERCOM_RX_PAD_1, UART_TX_PAD_0 ) ;             \
  void SERCOM##_sercom##_0_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercom##_1_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercom##_2_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercom##_3_Handler(void) {  Serial##_serialNum.IrqHandler(); }

// Use this form to specify the TX and RX pin numbers (Arduino Pin numbers.)
#define SERCOMLIB_MakeSerialPins(_sercom, _serialNum, _rx, _tx) \
  Uart Serial##_serialNum( &sercom##_sercom, _rx, _tx, SERCOM_RX_PAD_1, UART_TX_PAD_0 ) ; \
  void SERCOM##_sercom##_0_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercom##_1_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercom##_2_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercom##_3_Handler(void) {  Serial##_serialNum.IrqHandler(); }

// Use this form to specify the TX and RX pin numbers AND PAD designators
//  Note that on SAMD51, the TX pad is fixed.
#define SERCOMLIB_MakeSerialPinsPads(_sercom, _serialNum, _rx, _tx, _rxpad, _txpad) \
  Uart Serial##_serialNum( &sercom##_sercom, _rx, _tx, _txpad, _txpad ) ; \
  void SERCOM##_sercom##_0_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercom##_1_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercom##_2_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercom##_3_Handler(void) {  Serial##_serialNum.IrqHandler(); }

// Produce TwoWire name(&sercomN, PIN_sda, PIN_scl)
  #define SERCOMLIB_MakeWire(_sercom, _i2c, _sda, _scl) \
  TwoWire WIRE##_i2c(&sercom##_sercom, _sda, _scl); \
  void SERCOM##_sercom##_0_Handler(void) {  _i2c.onService(); } \
  void SERCOM##_sercom##_1_Handler(void) {  _i2c.onService(); } \
  void SERCOM##_sercom##_2_Handler(void) {  _i2c.onService(); } \
  void SERCOM##_sercom##_3_Handler(void) {  _i2c.onService(); }
#endif // SAMD51

#if SAMD21_SERIES || SAMD10_SERIES || SAMD11_SERIES || \
  defined(_SAMD21_) || defined(_SAMD10_) || defined(_SAMD11_)

// Use this form where there are pre-defined PIN_SERIALn_RX and
//  PIN_SERIALn_TX symbols
#define SERCOMLIB_MakeSerial(_sercom, _serialNum) \
  Uart Serial##_serialNum( &sercom##_sercom, PIN_Serial##_serialNum##_RX,  \
             PIN_SERIAL##_serialNum##_TX,                    \
             SERCOM_RX_PAD_1, UART_TX_PAD_0 ) ;              \
  void SERCOM##_sercom##_Handler(void) {   Serial##_serialNum.IrqHandler(); }

// Use this form to specify the TX and RX pin numbers (Arduino Pin numbers.)
#define SERCOMLIB_MakeSerialPins(_sercom, _serialNum, _rx, _tx) \
  Uart Serial##_serialNum( &sercom##_sercom, _rx, _tx, SERCOM_RX_PAD_1, UART_TX_PAD_0 ) ; \
  void SERCOM##_sercom##_Handler(void) { Serial##_serialNum.IrqHandler(); }

// Use this form to specify the TX and RX pin numbers AND PAD designators
#define SERCOMLIB_MakeSerialPinsPads(_sercom, _serialNum, _rx, _tx, _rxpad, _txpad) \
  Uart Serial##_serialNum( &sercom##_sercom, _rx, _tx, _rxpad, _txpad ) ; \
  void SERCOM##_sercom##_Handler(void) { Serial##_serialNum.IrqHandler(); }
#endif // SAMD21

/*
 * Some handy shortcuts.
 * I believe these should be consistant across boards, since SPI and I2C
 * are more limited in PAD usage.
 * They do require the MISO/etc pin names to be defined in the variant.
 */ 
#define SERCOMLIB_MakeSerial_OnWIRE(_sercom, _serialNum) \
  SERCOMLIB_MakeSerialPinsPads(_sercom, _serialNum, SCL, SDA, SERCOM_RX_PAD_1, UART_TX_PAD_0)

#define SERCOMLIB_MakeSerial_OnSPI(_sercom, _serialNum) \
  SERCOMLIB_MakeSerialPinsPads(_sercom, _serialNum, MISO, MOSI, SERCOM_RX_PAD_1, UART_TX_PAD_2)
