/*
 * sercomlib.h
 * April 2021, by Bill Westfield
 * Released to the public domain.
 *
 * This defines macros to pair up "un-used" sercom interfaces with
 *  Arduino driver-level definitions like UART/HardwareSerial
 *
 *  Use it like:
 *     sercom_MakeUart(sercomNumber, SerialNumber)
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

#if SAMD51_SERIES
// Use this form where there are pre-defined PIN_SERIALn_RX and
//   PIN_SERIALn_TX symbols
// Example: sercom_MakeUart(3, 4)
//          (create Serial4 bound to Sercom3)
#define sercom_MakeUart(_sercomNum, _serialNum)                         \
  Uart Serial##_serialNum( &sercom##_sercomNum, PIN_SERIAL##_serialNum##_RX, \
                           PIN_SERIAL##_serialNum##_TX,                 \
                           SERCOM_RX_PAD_1, UART_TX_PAD_0 ) ;           \
  void SERCOM##_sercomNum##_0_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercomNum##_1_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercomNum##_2_Handler(void) {  Serial##_serialNum.IrqHandler(); } \
  void SERCOM##_sercomNum##_3_Handler(void) {  Serial##_serialNum.IrqHandler(); }

// Use this form to specify the TX and RX pin numbers (Arduino Pin numbers.)
// Example: sercom_MakeUart_Pins(3, SerialXX, 1, 2)
//          (create SerialXX bound to Sercom3 on pins 1 and 2)
#define sercom_MakeUart_Pins(_sercomNum, _serName, _rx, _tx)            \
  Uart _serName( &sercom##_sercomNum, _rx, _tx, SERCOM_RX_PAD_1, UART_TX_PAD_0 ) ; \
  void SERCOM##_sercomNum##_0_Handler(void) {  _serName.IrqHandler(); } \
  void SERCOM##_sercomNum##_1_Handler(void) {  _serName.IrqHandler(); } \
  void SERCOM##_sercomNum##_2_Handler(void) {  _serName.IrqHandler(); } \
  void SERCOM##_sercomNum##_3_Handler(void) {  _serName.IrqHandler(); }

// Use this form to specify the TX and RX pin numbers AND PAD designators
//  Note that on SAMD51, the TX pad is fixed.
// Example: sercom_MakeUart_Pins(3, SerialXX, 1, 2)
//          (create SerialXX bound to Sercom3 on pins 1 and 2)
#define sercom_MakeUart_Pads(_sercomNum, _serName, _rx, _tx, _rxpad, _txpad) \
  Uart _serName( &sercom##_sercomNum, _rx, _tx, _txpad, _txpad ) ;      \
  void SERCOM##_sercomNum##_0_Handler(void) {  _serName.IrqHandler(); } \
  void SERCOM##_sercomNum##_1_Handler(void) {  _serName.IrqHandler(); } \
  void SERCOM##_sercomNum##_2_Handler(void) {  _serName.IrqHandler(); } \
  void SERCOM##_sercomNum##_3_Handler(void) {  _serName.IrqHandler(); }

// Produce TwoWire name(&sercomN, PIN_sda, PIN_scl)
#define sercom_MakeWire_Pins(_sercomNum, _i2c, _sda, _scl)              \
  TwoWire _i2c(&sercom##_sercomNum, _sda, _scl);                        \
  void SERCOM##_sercomNum##_0_Handler(void) {  _i2c.onService(); }      \
  void SERCOM##_sercomNum##_1_Handler(void) {  _i2c.onService(); }      \
  void SERCOM##_sercomNum##_2_Handler(void) {  _i2c.onService(); }      \
  void SERCOM##_sercomNum##_3_Handler(void) {  _i2c.onService(); }
#endif // SAMD51

#if SAMD21_SERIES || defined(_SAMD21_) || SAMD10_SERIES || SAMD11_SERIES

// Use this form where there are pre-defined PIN_SERIALn_RX and
//  PIN_SERIALn_TX symbols  (This is actually rare.)
#define sercom_MakeUart(_sercomNum, _serNumber)                         \
  Uart Serial##_serNumber( &sercom##_sercomNum, PIN_Serial##_serNumber##_RX, \
                           PIN_SERIAL##_serNumber##_TX,                 \
                           SERCOM_RX_PAD_1, UART_TX_PAD_0 ) ;           \
  void SERCOM##_sercomNum##_Handler(void) {  SERCOM##_serNumber.IrqHandler(); }

// Use this form to specify the TX and RX pin numbers (Arduino Pin numbers.)
#define sercom_MakeUart_Pins(_sercomNum, _serName, _rx, _tx)            \
  Uart _serName( &sercom##_sercomNum, _rx, _tx, SERCOM_RX_PAD_1, UART_TX_PAD_0 ) ; \
  void SERCOM##_sercomNum##_Handler(void) {  _serName.IrqHandler(); }

// Use this form to specify the TX and RX pin numbers AND PAD designators
#define sercom_MakeUart_Pads(_sercomNum, _serName, _rx, _tx, _rxpad, _txpad) \
  Uart _serName( &sercom##_sercomNum, _rx, _tx, _txpad, _txpad ) ;      \
  void SERCOM##_sercomNum##_Handler(void) {  _serName.IrqHandler(); }

#define sercom_MakeWire_Pins(_sercomNum, _i2cName, _sda, _scl)          \
  TwoWire _i2cName(&sercom##_sercomNum, _sda, _scl);                    \
  void SERCOM##_sercomNum##_Handler(void) {  _i2cName.onService(); }

#endif // SAMD21
