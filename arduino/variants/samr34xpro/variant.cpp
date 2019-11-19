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

#define NO_OTHER NOT_ON_TIMER, No_ADC_Channel, EXTERNAL_INT_NONE, GCLK_CCL_NONE

const PinDescription g_APinDescription[] = {
  /******* EDBG/UART */
  /* 00 */ {PORTA, 4, PIO_SERCOM, (PER_ATTR_SERCOM_ALT), (PIN_ATTR_SERCOM), NO_OTHER}, // TX
  /* 01 */ {PORTA, 5, PIO_SERCOM, (PER_ATTR_SERCOM_ALT), (PIN_ATTR_SERCOM), NO_OTHER}, // RX

  /******* LEDS */
  /* 02 */ {PORTA, 19, PIO_OUTPUT, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL), NO_OTHER}, // Y
  /* 03 */ {PORTA, 18, PIO_OUTPUT, (PER_ATTR_DRIVE_STRONG), (PIN_ATTR_DIGITAL), NO_OTHER}, // G

  /******* BUTTON */
  /* 04 */ {PORTA, 28, PIO_MULTI, (PER_ATTR_NONE), (PIN_ATTR_DIGITAL | PIN_ATTR_EXTINT), NOT_ON_TIMER, No_ADC_Channel, EXTERNAL_INT_8, GCLK_CCL_NONE},

   /******* OTHER */
};

const void *g_apTCInstances[TCC_INST_NUM + TC_INST_NUM] = {TCC0, TCC1, TCC2, TC3, TC4};

// Multi-serial objects instantiation
SERCOM sercom0(SERCOM0); // Serial
SERCOM sercom1(SERCOM1);
SERCOM sercom2(SERCOM2);
SERCOM sercom3(SERCOM3);
SERCOM sercom4(SERCOM4); // RESERVED FOR RF
SERCOM sercom5(SERCOM5);

Uart Serial(&sercom0, 1 /*RX_PIN*/, 0 /*TX_PIN*/, SERCOM_RX_PAD_1, UART_TX_PAD_0);
extern "C" void SERCOM0_Handler(void)
{
  Serial.IrqHandler();
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