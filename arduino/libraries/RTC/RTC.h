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
    RTCClass(int auto_begin)
    {
        if (auto_begin)
            begin();
    }

    void begin()
    {
        /* Turn on the digital interface clock */
        //system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBA, MCLK_APBAMASK_RTC);
        MCLK->APBAMASK.reg |= MCLK_APBAMASK_RTC;
        /* Select RTC clock */
        OSC32KCTRL->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC32K_Val;

        reset();
        enable();
        init();
    }

    void init(uint32_t prescaler = RTC_MODE0_CTRLA_PRESCALER_OFF_Val)
    {
        /* Set 32-bit mode and clear on match if applicable. */
        RTC->MODE0.CTRLA.reg |= RTC_MODE0_CTRLA_MODE(0) | (1 << RTC_MODE0_CTRLA_COUNTSYNC_Pos) | 0x1234; // prescaler ?
        wait_ready();
    }

    void disable()
    {
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
        RTC->MODE0.CTRLA.reg |= RTC_MODE0_CTRLA_ENABLE;
        wait_ready();
    }

    void reset()
    {
        disable();
        RTC->MODE0.CTRLA.reg |= RTC_MODE0_CTRLA_SWRST;
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