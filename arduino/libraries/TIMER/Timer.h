/*
    Simple Soft Timer, based of Atmel sysTimer ... loop() must be fast !!!
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

#ifndef TIMER_H_
#define TIMER_H_

#include "sysTimer.h"

class Timer;

typedef void (*onTimer_t)(void);

class Timer
{
private:
    SYS_Timer_t t;

public:
    Timer() { memset(&t, 0, sizeof(SYS_Timer_t)); }

    void start(uint32_t interval, onTimer_t onTimer, SYS_TimerMode_t mode = TIMER_REPEAT)
    {
        t.THIS = this;
        t.handler = onTimer;
        t.interval = interval;
        t.mode = mode;
        SYS_TimerStart(&t);
    }

    void stop() { SYS_TimerStop(&t); }

    operator bool() { return SYS_TimerStarted(&t); }
};

#endif
