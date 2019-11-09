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

unsigned int millis(void)
{
  return 0
}

unsigned int seconds(void)
{
  return millis() / 1000;
}

inline unsigned int micros()
{
  return 0;
}

inline void delayMicroseconds(unsigned int us)
{

}

void delay(unsigned int ms)
{

}

uint32_t clockCyclesPerMicrosecond(void)
{
    return 48000;
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

}

void noInterrupts(void)
{

}