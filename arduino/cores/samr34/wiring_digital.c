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
#include "WVariant.h"

#ifdef __cplusplus
extern "C"
{
#endif

  void pinMode(uint32_t ulPin, uint32_t ulMode)
  {
    pinPeripheral(ulPin, ulMode);
  }

  void digitalWrite(uint32_t ulPin, uint32_t ulVal)
  {
    uint8_t pinPort = GetPort(ulPin);
    if (pinPort == NOT_A_PORT)
      return;
    uint8_t pinNum = GetPin(ulPin);
    uint32_t pinAttribute = g_APinDescription[ulPin].ulPinAttribute;
    uint8_t pinConfig = PORT->Group[pinPort].PINCFG[pinNum].reg;
    uint8_t pinDir = (PORT->Group[pinPort].DIR.reg && (1ul << pinNum));
    uint8_t pinOut = (PORT->Group[pinPort].OUT.reg && (1ul << pinNum));
    if (pinDir == 0) // pin DIR is input
    {
      if (ulVal == HIGH)
      {
        if ((pinOut == 1 && (pinAttribute & PIN_ATTR_INPUT_PULLUP)) || (pinOut == 0 && (pinAttribute & PIN_ATTR_INPUT_PULLDOWN)))
        {
          pinConfig |= (uint8_t)(PORT_PINCFG_PULLEN);
        }
      }
      else
      {
        pinConfig &= ~(uint8_t)(PORT_PINCFG_PULLEN);
      }
      PORT->Group[pinPort].PINCFG[pinNum].reg = pinConfig;
    }
    else
    {
      if (ulVal == HIGH)
      {
        PORT->Group[pinPort].OUTSET.reg = (1ul << pinNum);
      }
      else
      {
        PORT->Group[pinPort].OUTCLR.reg = (1ul << pinNum);
      }
    }
    return;
  }

  int digitalRead(uint32_t ulPin)
  {
    uint8_t pinPort = GetPort(ulPin);
    if (pinPort == NOT_A_PORT)
      return LOW;
    uint8_t pinNum = GetPin(ulPin);
    if ((PORT->Group[pinPort].IN.reg & (1ul << pinNum)) != 0)
      return HIGH;
    return LOW;
  }

#ifdef __cplusplus
}
#endif
