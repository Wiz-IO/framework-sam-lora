/*
  SAMR3 - RFClass
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

#ifndef _RF_H_INCLUDED
#define _RF_H_INCLUDED

#include <SPI.h>

class RFClass : public SPIClass
{
public:
    RFClass() : SPIClass(&sercom4, RF_MISO, RF_SCK, RF_MOSI, (SercomSpiTXPad)1, (SercomRXPad)0) {}

    void begin()
    {
        SPIClass::begin();
        powerOnTCXO();
    }

    void powerOnTCXO()
    {
#ifdef RF_TCXO
        pinMode(RF_TCXO, OUTPUT);
        digitalWrite(RF_TCXO, 1);
        delay(2);
#endif
    }

    void powerOffTCXO()
    {
#ifdef RF_TCXO
        pinMode(RF_TCXO, INPUT);
#endif
    }

    void enableCtrl()
    {
#ifdef RF_SWITCH
        initCtrl();
        digitalWrite(RF_SWITCH, 1);
#endif
    }

    void disableCtrl()
    {
#ifdef RF_SWITCH
        initCtrl();
        digitalWrite(RF_SWITCH, 0);
#endif
    }

private:
    void initCtrl()
    {
#ifdef RF_SWITCH
        static int enabled = 0;
        if (0 == enabled)
        {
            pinMode(RF_SWITCH, OUTPUT);
            enabled = 1;
        }
#endif
    }
};

extern RFClass RF;

#endif
