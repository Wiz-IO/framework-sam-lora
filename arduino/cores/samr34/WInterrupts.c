/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  Copyright (c) 2017 MattairTech LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Arduino.h>
#include "wiring_private.h"
#include "WInterrupts.h"
#include "WVariant.h"
#include <string.h>

static voidFuncPtr ISRcallback[EXTERNAL_NUM_INTERRUPTS];
static uint32_t ISRlist[EXTERNAL_NUM_INTERRUPTS];
static uint32_t nints; // Stores total number of attached interrupts

/* Configure I/O interrupt sources */
static void __initialize()
{
  memset(ISRcallback, 0, sizeof(ISRcallback));
  memset(ISRlist, 0, sizeof(ISRlist));
  nints = 0;
  NVIC_DisableIRQ(EIC_IRQn);
  NVIC_ClearPendingIRQ(EIC_IRQn);
  NVIC_SetPriority(EIC_IRQn, 0);
  NVIC_EnableIRQ(EIC_IRQn);
  // Enable GCLK for IEC (External Interrupt Controller)
  MCLK->APBAMASK.reg |= MCLK_APBAMASK_EIC;
  GCLK->PCHCTRL[GCM_EIC].reg = (GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK0);
  while ((GCLK->PCHCTRL[GCM_EIC].reg & GCLK_PCHCTRL_CHEN) != GCLK_PCHCTRL_CHEN)
  { // wait for sync
  }
  //DBG("[EIC] enable\n");
}

void attachInterrupt(uint32_t pin, voidFuncPtr callback, uint32_t mode)
{
  static int enabled = 0;
  uint32_t config;
  uint32_t pos;

  //DBG("[attachInterrupt] pin %d\n", (int)pin);
  // The CHANGE and RISING interrupt modes on pin A31 on the SAML21 do not seem to work properly
  if ((GetPort(pin) == 0) && (GetPin(pin) == 31) && ((mode == CHANGE) || (mode == RISING)))
    return;

  EExt_Interrupts in = GetExtInt(pin);
  //DBG("[attachInterrupt] %d %d\n", (int)pin, (int)in);

#if defined(EXTERNAL_INT_NMI)
  if (in == NOT_AN_INTERRUPT || in == EXTERNAL_INT_NMI)
    return;
#else
  if (in == NOT_AN_INTERRUPT)
    return;
#endif

  if (!enabled)
  {
    __initialize();
    enabled = 1;
  }

  uint32_t inMask = 1 << in;

  // Enable wakeup capability on pin in case being used during sleep (WAKEUP always enabled on SAML and SAMC)

  // Assign pin to EIC
  if (pinPeripheral(pin, PIO_EXTINT) != RET_STATUS_OK)
    return;
  //DBG("[EIC] %d\n", (int)pin);

  // Only store when there is really an ISR to call.
  // This allow for calling attachInterrupt(pin, NULL, mode), we set up all needed register
  // but won't service the interrupt, this way we also don't need to check it inside the ISR.
  if (callback)
  {
    // Store interrupts to service in order of when they were attached
    // to allow for first come first serve handler
    uint32_t current = 0;

    // Check if we already have this interrupt
    for (current = 0; current < nints; current++)
    {
      if (ISRlist[current] == inMask)
      {
        break;
      }
    }
    if (current == nints)
    {
      nints++; // Need to make a new entry
    }
    ISRlist[current] = inMask;       // List of interrupt in order of when they were attached
    ISRcallback[current] = callback; // List of callback adresses

    // Look for right CONFIG register to be addressed
    if (in > EINT_7)
    {
      config = 1;
    }
    else
    {
      config = 0;
    }

    // Configure the interrupt mode
    pos = (in - (8 * config)) << 2; // compute position (ie: 0, 4, 8, 12, ...)

    // disable EIC before changing CONFIG
    EIC->CTRLA.reg = 0;
    while (EIC->SYNCBUSY.reg & EIC_SYNCBUSY_MASK)
    {
    }

    // copy register to variable, clearing mode bits
    uint32_t regConfig = (~(EIC_CONFIG_SENSE0_Msk << pos) & EIC->CONFIG[config].reg);

    // insert new mode and write to register (the hardware numbering for the 5 interrupt modes is in reverse order to the arduino numbering, so using '5-mode').
    EIC->CONFIG[config].reg = (regConfig | ((5 - mode) << pos));

    // enable EIC
    EIC->CTRLA.reg = EIC_CTRLA_ENABLE;
    while (EIC->SYNCBUSY.reg & EIC_SYNCBUSY_MASK)
    {
    }
  }

  // Clear the interrupt flag
  EIC->INTFLAG.reg = (inMask);

  // Enable the interrupt
  EIC->INTENSET.reg = (inMask);
}

/* Turns off the given interrupt. */
void detachInterrupt(uint32_t pin)
{
  EExt_Interrupts in = GetExtInt(pin);
  if (in == NOT_AN_INTERRUPT)
    return;

  uint32_t inMask = 1 << in;
  EIC->INTENCLR.reg = EIC_INTENCLR_EXTINT(inMask);

  // Remove callback from the ISR list
  uint32_t current;
  for (current = 0; current < nints; current++)
  {
    if (ISRlist[current] == inMask)
    {
      break;
    }
  }
  if (current == nints)
    return; // We didn't have it

  // Shift the reminder down
  for (; current < nints - 1; current++)
  {
    ISRlist[current] = ISRlist[current + 1];
    ISRcallback[current] = ISRcallback[current + 1];
  }
  nints--;
}

/* External Interrupt Controller NVIC Interrupt Handler */
void EIC_Handler(void)
{
  // Calling the routine directly from -here- takes about 1us
  // Depending on where you are in the list it will take longer
  // Loop over all enabled interrupts in the list
  for (uint32_t i = 0; i < nints; i++)
  {
    if ((EIC->INTFLAG.reg & ISRlist[i]) != 0)
    {
      // Call the callback function
      ISRcallback[i]();
      // Clear the interrupt
      EIC->INTFLAG.reg = ISRlist[i];
    }
  }
  //DBG("[EIC]\n");
}
