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

#include <Arduino.h>
#include "WVariant.h"

#define NOT_CONNECTED NOT_A_PORT, 128, 128, 128, 128, 128, 128, 128
#define NO_OTHER      TIMER_NONE, ADC_NONE, EINT_NONE, GCLK_CCL_NONE

const PinDescription g_APinDescription[] = {
/******* ALL PINS **************************************************************************************************************************/
  /* 00 */ {NOT_CONNECTED}, // PA00 - XIN32
  /* 01 */ {NOT_CONNECTED}, // PA01 - XOUT32
  /* 02 */ {PORTA,  4, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 03 */ {PORTA,  5, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 04 */ {PORTA,  6, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 05 */ {PORTA,  7, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 06 */ {PORTA,  8, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 07 */ {PORTA,  9, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 08 */ {PORTA, 13, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 09 */ {NOT_CONNECTED}, // A14 - XIN
  /* 10 */ {NOT_CONNECTED}, // A15 - XOUT
  /* 11 */ {PORTA, 16, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 12 */ {PORTA, 17, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 13 */ {PORTA, 18, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 14 */ {PORTA, 19, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 15 */ {PORTA, 22, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 16 */ {PORTA, 23, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 17 */ {PORTA, 24, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 18 */ {PORTA, 25, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 19 */ {PORTA, 27, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 20 */ {PORTA, 28, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 21 */ {PORTA, 30, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 22 */ {PORTA, 31, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 23 */ {PORTB, 02, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 24 */ {PORTB, 03, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 25 */ {PORTB, 22, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},
  /* 26 */ {PORTB, 23, PIO_MULTI, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_0, GCLK_CCL_NONE},

/******* BOARD CONSTANT *********************************************************************************************************************/
/******* EDBG - Serial */
  /* 27 */ {PORTA, 4, PIO_SERCOM, (PER_ATTR_SERCOM_ALT), (PIN_ATTR_SERCOM), NO_OTHER}, // TX: SERCOM0-0
  /* 28 */ {PORTA, 5, PIO_SERCOM, (PER_ATTR_SERCOM_ALT), (PIN_ATTR_SERCOM), NO_OTHER}, // RX: SERCOM0-1

/******* LEDS */
  /* 29 */ {PORTA, 19, PIO_OUTPUT, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL), NO_OTHER},
  /* 30 */ {PORTA, 18, PIO_OUTPUT, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL), NO_OTHER},

/******* BUTTON */
  /* 31 */ {PORTA, 28, PIO_MULTI, (PER_ATTR_NONE), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), TIMER_NONE, ADC_NONE, EINT_8, GCLK_CCL_NONE},

/******* SERCOMS  ****************************************************************************************************************************/
/******* UART - Serial1  */
  /* 32 */ {PORTA, 16, PIO_MULTI, (PER_ATTR_SERCOM_ALT), (PIN_ATTR_SERCOM), NO_OTHER}, //SS1-0, SA3-0
  /* 33 */ {PORTA, 17, PIO_MULTI, (PER_ATTR_SERCOM_ALT), (PIN_ATTR_SERCOM), NO_OTHER}, //SS1-0, SA3-0

/******* I2C */
  /* 34 */ {PORTA, 16, PIO_MULTI, (PER_ATTR_SERCOM_STD), (PIN_ATTR_SERCOM), NO_OTHER}, //SS1-0, SA3-0
  /* 35 */ {PORTA, 17, PIO_MULTI, (PER_ATTR_SERCOM_STD), (PIN_ATTR_SERCOM), NO_OTHER}, //SS1-1, SA3-1

/******* SPI PINS */
  /* 36 */ {PORTB, 22, PIO_MULTI, (PER_ATTR_SERCOM_ALT), (PIN_ATTR_DIGITAL | PIN_ATTR_SERCOM), NO_OTHER},//MOSI
  /* 37 */ {PORTB,  2, PIO_MULTI, (PER_ATTR_SERCOM_ALT), (PIN_ATTR_DIGITAL | PIN_ATTR_SERCOM), NO_OTHER},//MISO
  /* 38 */ {PORTB, 23, PIO_MULTI, (PER_ATTR_SERCOM_ALT), (PIN_ATTR_DIGITAL | PIN_ATTR_SERCOM), NO_OTHER},//SCK
  /* 39 */ {PORTB,  3, PIO_MULTI, (PER_ATTR_DRIVE_STRONG | PER_ATTR_SERCOM_ALT), (PIN_ATTR_DIGITAL), NO_OTHER},//SS

/******* INTERNAL CONNECTED ****************************************************************************************************************/

};

const void *g_apTCInstances[TCC_INST_NUM + TC_INST_NUM] = {TCC0, TCC1, TCC2, TC3, TC4};

SERCOM sercom0(SERCOM0); // SERIAL
SERCOM sercom1(SERCOM1); // I2C
SERCOM sercom2(SERCOM2);
SERCOM sercom3(SERCOM3); // serial1
SERCOM sercom4(SERCOM4); // RESERVED FOR RF
SERCOM sercom5(SERCOM5); // spi

Uart Serial(&sercom0, PIN_SERIAL_RX, PIN_SERIAL_TX, SERCOM_RX_PAD_1, UART_TX_PAD_0);
extern "C" void SERCOM0_Handler(void)
{
  Serial.IrqHandler();
}

Uart Serial1(&sercom3, 33, 32, SERCOM_RX_PAD_1, UART_TX_PAD_0);
extern "C" void SERCOM3_Handler(void)
{
  Serial1.IrqHandler();
}

void initVariant()
{
  // Clock SERCOM for Serial, TC/TCC for Pulse and Analog, and ADC/DAC for Analog
  uint32_t regAPBCMASK = MCLK->APBCMASK.reg;

  regAPBCMASK |= MCLK_APBCMASK_SERCOM0 | MCLK_APBCMASK_SERCOM1 | MCLK_APBCMASK_SERCOM2 | MCLK_APBCMASK_SERCOM3 | MCLK_APBCMASK_SERCOM4;

  MCLK->APBDMASK.reg |= MCLK_APBDMASK_SERCOM5 | MCLK_APBDMASK_TC4; // On the SAML, SERCOM5 and TC4 are on the low power bridge
  regAPBCMASK |= MCLK_APBCMASK_TCC0 | MCLK_APBCMASK_TCC1 | MCLK_APBCMASK_TCC2 | MCLK_APBCMASK_TC0 | MCLK_APBCMASK_TC1 | MCLK_APBCMASK_TC2;

  regAPBCMASK |= MCLK_APBCMASK_DAC;
  MCLK->APBDMASK.reg |= MCLK_APBDMASK_ADC; // On the SAML, ADC is on the low power bridge

  MCLK->APBCMASK.reg |= regAPBCMASK;

  // Setup all pins (digital and analog) in STARTUP mode (enable INEN and set default pull direction to pullup (pullup will not be enabled))
  //for (uint32_t ul = 0; ul < NUM_DIGITAL_PINS; ul++) pinMode(ul, PIO_STARTUP);

  //initADC();                           // Initialize Analog Controller
  //analogReference(VARIANT_AR_DEFAULT); // Use default reference from variant.h
  //initDAC();                           // Initialize DAC
}