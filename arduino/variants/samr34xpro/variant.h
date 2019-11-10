/*
  SAMR3 - variant
    Created on: 01.01.2020
    Author: Georgi Angelov

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA   
 */

#ifndef __VARIANT_H__
#define __VARIANT_H__

#ifdef __cplusplus
extern "C"
{
#endif
// C ware //////////////////////////////////////////////////////////////////////////

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define LED (2) /* from table */
#define LED_BUILTIN LED

#define PINS_COUNT            (26u)
#define NUM_DIGITAL_PINS      (20u)
#define NUM_ANALOG_INPUTS     (6u)
#define NUM_ANALOG_OUTPUTS    (1u)

#define PIN_SERIAL_RX         (0ul)
#define PIN_SERIAL_TX         (1ul)
#define PAD_SERIAL_TX         (UART_TX_PAD_0)
#define PAD_SERIAL_RX         (SERCOM_RX_PAD_1)

int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);

#define analogInputToDigitalPin(p)  ((p < 6u) ? (p) + 14u : -1)
#define digitalPinToPort(P)			    ( &(PORT->Group[g_APinDescription[P].ulPort]) )
#define digitalPinToBitMask(P)		  ( 1 << g_APinDescription[P].ulPin )
#define portOutputRegister(port)	  ( &(port->OUT.reg) )
#define portInputRegister(port)		  ( &(port->IN.reg) )
#define portModeRegister(port)		  ( &(port->DIR.reg) )
#define digitalPinHasPWM(P)			    ( g_APinDescription[P].ulPWMChannel != NOT_ON_PWM || g_APinDescription[P].ulTCChannel != NOT_ON_TIMER )
#define digitalPinToInterrupt(P)	  (g_APinDescription[P].ulExtInt)


#ifdef __cplusplus
}
// CPP ware ////////////////////////////////////////////////////////////////////////

void initVariant();

#include <Sercom.h>
extern SERCOM sercom0;
extern SERCOM sercom1;
extern SERCOM sercom2;
extern SERCOM sercom3;
extern SERCOM sercom4; // RESERVED FOR RF
extern SERCOM sercom5;

#include <Uart.h>
extern Uart Serial;

#endif //__cplusplus

#endif /* __VARIANT_H__ */