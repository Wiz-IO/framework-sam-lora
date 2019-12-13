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
private:
    int started;
    void wait_busy()
    {
        while (RTC->MODE0.SYNCBUSY.reg)
        {
        }
    }

    /*
        Default function ... I dont know user pin/mux config
        This make 65 uA on sleep backup mode
    */
    void kill_ports(void)
    {
        uint32_t cfg = (0xFFFF << PORT_WRCONFIG_PINMASK_Pos) | PORT_WRCONFIG_WRPMUX | PORT_WRCONFIG_WRPINCFG;
        PORT->Group[0].WRCONFIG.reg = cfg;
        PORT->Group[0].WRCONFIG.reg = cfg | PORT_WRCONFIG_HWSEL;
        //PORT->Group[0].DIRCLR.reg = -1;
        PORT->Group[1].WRCONFIG.reg = cfg;
        PORT->Group[1].WRCONFIG.reg = cfg | PORT_WRCONFIG_HWSEL;
        //PORT->Group[1].DIRCLR.reg = -1;
        PORT->Group[2].WRCONFIG.reg = cfg;
        PORT->Group[2].WRCONFIG.reg = cfg | PORT_WRCONFIG_HWSEL;
        //PORT->Group[2].DIRCLR.reg = -1;
    }

public:
    RTCClass() : started(0) {}
    RTCClass(int start)
    {
        started = 0;
        begin();
    }

    void begin()
    {
        if (started)
            return;
        /* Turn on the digital interface clock */
        MCLK->APBAMASK.reg |= MCLK_APBAMASK_RTC;
        /* Select RTC clock: 1.024kHz from 32KHz external oscillator  */
        OSC32KCTRL->RTCCTRL.bit.RTCSEL = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC1K_Val;
        /* Reset module to hardware defaults. */
        reset();
        /* Setup and Enable. */
        while (0 == (RTC->MODE0.CTRLA.reg & RTC_MODE0_CTRLA_ENABLE)) // ?!?
        {
            RTC->MODE0.CTRLA.reg = RTC_MODE0_CTRLA_ENABLE |
                                   RTC_MODE0_CTRLA_MODE(0) |
                                   RTC_MODE0_CTRLA_COUNTSYNC |
                                   RTC_MODE0_CTRLA_PRESCALER_DIV1024; // one second
            wait_busy();
        }
        started = 1;
    }

    void reset()
    {
        disable(); // before soft-reset
        RTC->MODE0.CTRLA.reg |= RTC_MODE0_CTRLA_SWRST;
        while (RTC->MODE0.SYNCBUSY.reg || RTC->MODE0.CTRLA.bit.SWRST)
        {
        }
    }

    void disable()
    {
        wait_busy();
        /* Disbale interrupt */
        RTC->MODE0.INTENCLR.reg = RTC_MODE0_INTENCLR_MASK;
        /* Clear interrupt flag */
        RTC->MODE0.INTFLAG.reg = RTC_MODE0_INTFLAG_MASK;
        /* Disable RTC module. */
        RTC->MODE0.CTRLA.reg &= ~RTC_MODE0_CTRLA_ENABLE;
        wait_busy();
    }

    void set(uint32_t value)
    {
        if (0 == started)
            begin();
        wait_busy();
        RTC->MODE0.COUNT.reg = value;
        wait_busy();
    }

    inline uint32_t get()
    {
        if (0 == started)
            begin();
        wait_busy();
        return RTC->MODE0.COUNT.reg;
    }

    void set_compare(uint32_t index, uint32_t value)
    {
        if (0 == started)
            begin();
        if (index > RTC_COMP32_NUM)
            abort();
        wait_busy();
        RTC->MODE0.COMP[index].reg = value;
        wait_busy();
    }

    uint32_t get_compare(uint32_t index)
    {
        if (0 == started)
            begin();
        if (index > RTC_COMP32_NUM)
            abort();
        wait_busy();
        return RTC->MODE0.COMP[index].reg;
    }

    /* 
        Enter in sleep and wakeup after seconds 
        use before_sleep() to "kill" your ports pin/mux values
    */
    void sleep_wakeup(uint32_t second, void (*before_sleep)(void) = NULL)
    {
        if (0 == started)
            begin();
        if (0 == second)
            second = 1;

        if (NULL == before_sleep)
            kill_ports();
        else
            before_sleep();

        NVIC_DisableIRQ(RTC_IRQn);
        set_compare(0, get() + second);
        NVIC_ClearPendingIRQ(RTC_IRQn);
        NVIC_SetPriority(RTC_IRQn, 0);
        RTC->MODE0.INTFLAG.reg = -1;
        NVIC_EnableIRQ(RTC_IRQn);
        RTC->MODE0.INTENSET.bit.CMP0 = 1;

        sys_set_sleep_mode(SLEEP_MODE_BACKUP);
        sys_sleep();
    }

      /*
        Store max uint32[ 4 ] values to rtc ram
        rtc.set_backup(1, val_1);
          ...
        rtc.set_backup(4, val_1, val_2, val_3, val_4);
    */
    void set_backup(int n_values, ...)
    {
        n_values &= 3;
        va_list list;
        va_start(list, n_values);
        for (int i = 0; i < n_values; i++)
            RTC->MODE0.GP[i].reg = va_arg(list, uint32_t);
        va_end(list);
    }

    /*
        Restore max uint32[ 4 ] values from rtc ram
        rtc.get_backup(1, &val_1);
          ...
        rtc.get_backup(4, &val_1, &val_2, &val_3, &val_4);
    */
    void get_backup(int n_values, ...)
    {
        n_values &= 3;
        va_list list;
        va_start(list, n_values);
        for (int i = 0; i < n_values; i++)
        {
            uint32_t *p = va_arg(list, uint32_t *);
            if (p)
                *p = RTC->MODE0.GP[i].reg;
        }
        va_end(list);
    }
  
};

#endif
