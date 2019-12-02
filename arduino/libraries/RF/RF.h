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

  Notes:
    RF is child class of SPI - SERCOM4
    RF not have power domain
 */

#ifndef _RF_H_INCLUDED
#define _RF_H_INCLUDED

#include <SPI.h>

extern "C" void pinMux(int pin, int peripheral);

class RFClass : public SPIClass
{
public:
    RFClass() : SPIClass(&sercom4, RF_MISO, RF_SCK, RF_MOSI, (SercomSpiTXPad)1, (SercomRXPad)0)
    {
        _started = 0;
    }

    void begin()
    {
        if (0 == _started)
        {
            enableOscilator();
            SPIClass::begin();
            reset();
            _started = 1;
        }
    }

    void end() // goto full sleep
    {
        if (_started)
        {
            sleep();
        }
        else
        {
            begin();
            sleep();
        }
        SPIClass::end();        
        disableOscilator();
        disableSwitch();
        digitalWrite(RF_SEL, 0);     
        _started = 0;
    }

    void enableOscilator()
    {
        initOscilator();
        digitalWrite(RF_TCXO, 1);
        delay(1);
    }

    void disableOscilator()
    {
        initOscilator();
        digitalWrite(RF_TCXO, 0);
        delay(1);
    }

    void enableSwitch()
    {
        initSwitch();
        digitalWrite(RF_SWITCH, 1);
    }

    void disableSwitch()
    {
        initSwitch();
        digitalWrite(RF_SWITCH, 0);
    }

    void reset()
    {
        static int once = 0;
        if (0 == once)
            pinMode(RF_SEL, OUTPUT);
        digitalWrite(RF_SEL, 1);
        delay(1);
        if (0 == once)
            pinMode(RF_RST, OUTPUT);
        digitalWrite(RF_RST, 0);
        delay(10);
        digitalWrite(RF_RST, 1);
        delay(10);
        once = 1;
    }

    int version()
    {
        return (0x12 == readRegister(0x42)); // REG_VERSION ... is 0x12
    }

    void sleep()
    {
        while (0 != readRegister(1))
            writeRegister(1, 0);
    }

    uint8_t readRegister(uint8_t address)
    {
        return singleTransfer(address & 0x7f, 0x00);
    }

    void writeRegister(uint8_t address, uint8_t value)
    {
        singleTransfer(address | 0x80, value);
    }

    friend class LoRaClass;

private:
    int _started;

    void initSwitch()
    {
        static int once = 0;
        if (once)
            return;
        pinMode(RF_SWITCH, OUTPUT);
        once = 1;
    }

    void initOscilator()
    {
        static int once = 0;
        if (once)
            return;
        pinMode(RF_TCXO, OUTPUT);
        once = 1;
    }

protected:
    uint8_t singleTransfer(uint8_t address, uint8_t value)
    {
        if (0 == _started)
            return 0;
        uint8_t response;
        digitalWrite(RF_SEL, 0);
        beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
        transfer(address);
        response = transfer(value);
        endTransaction();
        digitalWrite(RF_SEL, 1);
        return response;
    }
};

extern RFClass RF;

#endif
