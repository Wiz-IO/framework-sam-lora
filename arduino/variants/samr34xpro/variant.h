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

#include <wiring_analog.h>

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

#define VARIANT_MCK (48000000ul)

#define NUM_PIN_DESCRIPTION_ENTRIES (63u) 

/* LED from table */
#define LED_ON                (0) 
#define LED_OFF               (1) 

#define LED                   (29) 
#define LED_BUILTIN           LED
#define LED_Y                 LED
#define LED_G                 (30) 
#define BUTTON                (31) 

/* Analog pins    TODO */
#define PIN_A0               (14ul)
#define PIN_A1               (15ul)
#define PIN_A2               (16ul)
#define PIN_A3               (17ul)
#define PIN_A4               (18ul)
#define PIN_A5               (19ul)
#define PIN_DAC0             (14ul)
static const uint8_t A0     = PIN_A0;
static const uint8_t A1     = PIN_A1;
static const uint8_t A2     = PIN_A2;
static const uint8_t A3     = PIN_A3;
static const uint8_t DAC0   = PIN_DAC0;
#define ADC_RESOLUTION		  12
#define REMAP_ANALOG_PIN_ID(pin)	if ( pin < A0 ) pin += A0
/* Set default analog voltage reference */
#define VARIANT_AR_DEFAULT	AR_DEFAULT  
  
/* Serial from table */
#define PIN_SERIAL_TX         (27) 
#define PIN_SERIAL_RX         (28) 


/* I2C from table */
#define PIN_WIRE_SDA          (34) 
#define PIN_WIRE_SCL          (35) 

/* SPI from table */
#define PIN_SPI_MOSI          (36)
#define PIN_SPI_MISO          (37)
#define PIN_SPI_SCK           (38)
#define PIN_SPI_SS            (39)

/* RF from table */
#define RF_SEL                (50)  
#define RF_MOSI               (51)  
#define RF_MISO               (52)  
#define RF_SCK                (53)  
#define RF_RST                (54)   
#define RF_DIO0               (55)
#define RF_DIO1               (56)
#define RF_DIO2               (57)
#define RF_DIO3               (58)
#define RF_DIO4               (59)
#define RF_DIO5               (60)
#define RF_TCXO               (61)
#define RF_SWITCH             (62)

/* * */

#define analogInputToDigitalPin(p)  ((p < 6u) ? (p) + 14u : -1)
#define digitalPinToPort(P)         ( &(PORT->Group[g_APinDescription[P].ulPort]) )
#define digitalPinToBitMask(P)      ( 1 << g_APinDescription[P].ulPin )
#define portOutputRegister(port)    ( &(port->OUT.reg) )
#define portInputRegister(port)     ( &(port->IN.reg) )
#define portModeRegister(port)      ( &(port->DIR.reg) )
#define digitalPinHasPWM(P)         ( g_APinDescription[P].ulPWMChannel != NOT_ON_PWM || g_APinDescription[P].ulTCChannel != NOT_ON_TIMER )
#define digitalPinToInterrupt(P)    ( P )

extern uint32_t SystemCoreClock;

int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);

#ifdef __cplusplus
}

// CPP ware ////////////////////////////////////////////////////////////////////////

void initVariant();

#include <SERCOM.h>
extern SERCOM sercom0; // Serial
extern SERCOM sercom1; // I2C
extern SERCOM sercom2;
extern SERCOM sercom3; // Serial1
extern SERCOM sercom4; // RESERVED FOR RF
extern SERCOM sercom5; // SPI

#include <Uart.h>
extern Uart Serial;
extern Uart Serial1;

#define SAMR34XPRO /* for libraries */

#endif //__cplusplus
#endif /* __VARIANT_H__ */
