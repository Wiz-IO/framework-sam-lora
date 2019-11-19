// NOT TESTED

/*
  SAMR3 - RTCClass
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

#ifndef RTC_H
#define RTC_H

#include <Arduino.h>
#include <samr3.h>

class RTCClass
{
public:
    RTCClass() {}

    void begin()
    {
        /* Turn on the digital interface clock */
        MCLK->APBAMASK.reg |= MCLK_APBAMASK_RTC; // system_apb_clock_set_mask(0, 0x100);
        /* Select RTC clock */
        OSC32KCTRL->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC32K_Val; //OSC32KCTRL_RTCCTRL_RTCSEL_XOSC1K_Val
        /* Reset module to hardware defaults. */
        reset();

        /* Set 32-bit mode and clear on match if applicable. */
        RTC->MODE0.CTRLA.reg |= RTC_MODE0_CTRLA_MODE(0) | /* COUNT32 */
                                //RTC_MODE0_CTRLA_MATCHCLR |
                                RTC_MODE0_CTRLA_COUNTSYNC |
                                RTC_MODE0_CTRLA_PRESCALER_OFF_Val; //RTC_MODE0_CTRLA_PRESCALER_DIV1024_Val
        enable();
    }

    void disable()
    {
        wait_ready();
        /* Disbale interrupt */
        RTC->MODE0.INTENCLR.reg = RTC_MODE0_INTENCLR_MASK;
        /* Clear interrupt flag */
        RTC->MODE0.INTFLAG.reg = RTC_MODE0_INTFLAG_MASK;
        /* Disable RTC module. */
        RTC->MODE0.CTRLA.reg &= ~RTC_MODE0_CTRLA_ENABLE;
        wait_ready();
    }

    void enable()
    {
        /* Enable RTC module. */
        wait_ready();
        RTC->MODE0.CTRLA.reg |= RTC_MODE0_CTRLA_ENABLE;
        wait_ready();
        Serial.printf("CTRLA:  %08X\n", RTC->MODE0.CTRLA.reg);
    }

    void reset()
    {
        disable();
        wait_ready();
        RTC->MODE0.CTRLA.bit.SWRST = 1;
        wait_ready();        
    }

    void set(uint32_t value)
    {
        RTC->MODE0.COUNT.reg = value;
        wait_ready();
    }

    inline uint32_t get() { return RTC->MODE0.COUNT.reg; }

    void set_compare(uint32_t index, uint32_t value)
    {
        if (index > RTC_COMP32_NUM)
            abort();
        RTC->MODE0.COMP[index].reg = value;
        wait_ready();
    }

    uint32_t get_compare(uint32_t index)
    {
        if (index > RTC_COMP32_NUM)
            abort();
        return RTC->MODE0.COMP[index].reg;
    }

    //start(uint32_t sleepTicks, void (*cb)(void)){}
    //stop(){}

private:
    void wait_ready()
    {
        while (RTC->MODE0.SYNCBUSY.reg)
        {
        }
    }
};

extern RTCClass rtc;

#endif