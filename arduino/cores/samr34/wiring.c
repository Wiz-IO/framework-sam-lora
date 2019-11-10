/*
  SAMR3 - wiring
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
#include <variant.h>

/** Tick Counter united by ms */
static volatile uint32_t _ulTickCount = 0;

void SysTick_Handler(void)
{
  // Increment tick count each ms
  _ulTickCount++;
}

unsigned int millis(void)
{
  // todo: ensure no interrupts
  return _ulTickCount;
}

// Interrupt-compatible version of micros
// Theory: repeatedly take readings of SysTick counter, millis counter and SysTick interrupt pending flag.
// When it appears that millis counter and pending is stable and SysTick hasn't rolled over, use these
// values to calculate micros. If there is a pending SysTick, add one to the millis counter in the calculation.
unsigned int micros(void)
{
  uint32_t ticks, ticks2;
  uint32_t pend, pend2;
  uint32_t count, count2;

  ticks2 = SysTick->VAL;
  pend2 = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk);
  count2 = _ulTickCount;

  do
  {
    ticks = ticks2;
    pend = pend2;
    count = count2;
    ticks2 = SysTick->VAL;
    pend2 = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk);
    count2 = _ulTickCount;
  } while ((pend != pend2) || (count != count2) || (ticks < ticks2));

  return ((count + pend) * 1000) + (((SysTick->LOAD - ticks) * (1048576 / (VARIANT_MCK / 1000000))) >> 20);
  // this is an optimization to turn a runtime division into two compile-time divisions and
  // a runtime multiplication and shift, saving a few cycles
}

unsigned int seconds(void)
{
  return millis() / 1000;
}

inline void delayMicroseconds(unsigned int us)
{
  // TODO
}

void delay(unsigned int ms)
{
  if (ms == 0)
  {
    return;
  }
  uint32_t start = _ulTickCount;
  do
  {
    // __asm__ __volatile__("");
  } while (_ulTickCount - start < ms);
}

uint32_t clockCyclesPerMicrosecond(void)
{
  return VARIANT_MCK / 1000;
}

uint32_t clockCyclesToMicroseconds(uint32_t a)
{
  return a / clockCyclesPerMicrosecond();
}

uint32_t microsecondsToClockCycles(uint32_t a)
{
  return a * clockCyclesPerMicrosecond();
}

void interrupts(void)
{
  // TODO
}

void noInterrupts(void)
{
  // TODO
}

int pinPeripheral(uint32_t ulPin, EPioType ulPeripheral)
{
  // Handle the case the pin isn't usable as PIO
  if (g_APinDescription[ulPin].ulPinType == PIO_NOT_A_PIN)
  {
    return -1;
  }

  switch (ulPeripheral)
  {
  case PIO_DIGITAL:
  case PIO_INPUT:
  case PIO_INPUT_PULLUP:
  case PIO_OUTPUT:
    // Disable peripheral muxing, done in pinMode
    //		PORT->Group[g_APinDescription[ulPin].ulPort].PINCFG[g_APinDescription[ulPin].ulPin].bit.PMUXEN = 0 ;

    // Configure pin mode, if requested
    if (ulPeripheral == PIO_INPUT)
    {
      pinMode(ulPin, INPUT);
    }
    else
    {
      if (ulPeripheral == PIO_INPUT_PULLUP)
      {
        pinMode(ulPin, INPUT_PULLUP);
      }
      else
      {
        if (ulPeripheral == PIO_OUTPUT)
        {
          pinMode(ulPin, OUTPUT);
        }
        else
        {
          // PIO_DIGITAL, do we have to do something as all cases are covered?
        }
      }
    }
    break;

  case PIO_ANALOG:
  case PIO_SERCOM:
  case PIO_SERCOM_ALT:
  case PIO_TIMER:
  case PIO_TIMER_ALT:
  case PIO_EXTINT:
  case PIO_COM:
  case PIO_AC_CLK:
#if 0
      // Is the pio pin in the lower 16 ones?
      // The WRCONFIG register allows update of only 16 pin max out of 32
      if ( g_APinDescription[ulPin].ulPin < 16 )
      {
        PORT->Group[g_APinDescription[ulPin].ulPort].WRCONFIG.reg = PORT_WRCONFIG_WRPMUX | PORT_WRCONFIG_PMUXEN | PORT_WRCONFIG_PMUX( ulPeripheral ) |
                                                                    PORT_WRCONFIG_WRPINCFG |
                                                                    PORT_WRCONFIG_PINMASK( g_APinDescription[ulPin].ulPin ) ;
      }
      else
      {
        PORT->Group[g_APinDescription[ulPin].ulPort].WRCONFIG.reg = PORT_WRCONFIG_HWSEL |
                                                                    PORT_WRCONFIG_WRPMUX | PORT_WRCONFIG_PMUXEN | PORT_WRCONFIG_PMUX( ulPeripheral ) |
                                                                    PORT_WRCONFIG_WRPINCFG |
                                                                    PORT_WRCONFIG_PINMASK( g_APinDescription[ulPin].ulPin - 16 ) ;
      }
#else
    if (g_APinDescription[ulPin].ulPin & 1) // is pin odd?
    {
      uint32_t temp;
      // Get whole current setup for both odd and even pins and remove odd one
      temp = (PORT->Group[g_APinDescription[ulPin].ulPort].PMUX[g_APinDescription[ulPin].ulPin >> 1].reg) & PORT_PMUX_PMUXE(0xF);
      // Set new muxing
      PORT->Group[g_APinDescription[ulPin].ulPort].PMUX[g_APinDescription[ulPin].ulPin >> 1].reg = temp | PORT_PMUX_PMUXO(ulPeripheral);
      // Enable port mux
      PORT->Group[g_APinDescription[ulPin].ulPort].PINCFG[g_APinDescription[ulPin].ulPin].reg |= PORT_PINCFG_PMUXEN;
    }
    else // even pin
    {
      uint32_t temp;
      temp = (PORT->Group[g_APinDescription[ulPin].ulPort].PMUX[g_APinDescription[ulPin].ulPin >> 1].reg) & PORT_PMUX_PMUXO(0xF);
      PORT->Group[g_APinDescription[ulPin].ulPort].PMUX[g_APinDescription[ulPin].ulPin >> 1].reg = temp | PORT_PMUX_PMUXE(ulPeripheral);
      PORT->Group[g_APinDescription[ulPin].ulPort].PINCFG[g_APinDescription[ulPin].ulPin].reg |= PORT_PINCFG_PMUXEN; // Enable port mux
    }
#endif
    break;

  case PIO_NOT_A_PIN:
    return -1l;
    break;
  }

  return 0l;
}
