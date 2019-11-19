#include <stdint.h>
#include <samr34.h>

/*
     We need XOSC32K to 48MHz
        and RTC counter 1 seconds
*/

/*
  SAMR3 - oscilator
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

uint32_t SystemCoreClock = 48000000ul; // must be set

#define GCLK_DFLL48M_REF 0

static inline int gclk_is_syncing(const uint8_t generator)
{
    return (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(generator));
}

void gclk_setup(int generator, uint32_t reg)
{
    while (gclk_is_syncing(generator))
    {
    }
    GCLK->GENCTRL[generator].reg = reg;
    while (gclk_is_syncing(generator))
    {
    }
}

void waitForDFLL(void)
{
    while ((OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY) == 0)
    {
    }
}

void waitForPLL(void)
{
    while (OSCCTRL->DPLLSYNCBUSY.reg & OSCCTRL_DPLLSYNCBUSY_MASK)
    {
    }
}

void init_clock(void)
{
    /* disable the watchdog timer */
    WDT->CTRLA.bit.ENABLE = 0;

    /* Various bits in the INTFLAG register can be set to one at startup. This will ensure that these bits are cleared */
    OSCCTRL->INTFLAG.reg = OSCCTRL_INTFLAG_DFLLRDY;
    SUPC->INTFLAG.reg = SUPC_INTFLAG_BOD33RDY | SUPC_INTFLAG_BOD33DET;

    /* FLASH */
    NVMCTRL->CTRLB.bit.RWS = 2;         // wait states
    PM->INTFLAG.reg = PM_INTFLAG_PLRDY; /* Switch to PL2 to be sure configuration of GCLK0 is safe */
    PM->PLCFG.reg = PM_PLCFG_PLSEL_PL2; /* Switch performance level = SYSTEM_PERFORMANCE_LEVEL_2 */
    while (!PM->INTFLAG.reg)
    { /* Waiting performance level ready */
    }

    /* Software reset the GCLK module to ensure it is re-initialized correctly */
    GCLK->CTRLA.reg = GCLK_CTRLA_SWRST;
    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST)
    { /* Wait for synchronization */
    }

    /* Enable external XOSC32K oscilator */
    OSC32KCTRL->XOSC32K.reg = (OSC32KCTRL_XOSC32K_STARTUP(0x3u/*1s*/) | OSC32KCTRL_XOSC32K_XTALEN | OSC32KCTRL_XOSC32K_EN32K | OSC32KCTRL_XOSC32K_EN1K);
    OSC32KCTRL->XOSC32K.bit.ENABLE = 1;
    while ((OSC32KCTRL->STATUS.reg & OSC32KCTRL_STATUS_XOSC32KRDY) == 0)
    { /* Wait for oscillator stabilization */
    }

    /* Set 32k to external oscilator */
    gclk_setup(1, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_XOSC32K); // ???GCLK_GENCTRL_SRC_OSCULP32K
    GCLK->PCHCTRL[GCLK_DFLL48M_REF /*0*/].reg = GCLK_PCHCTRL_GEN_GCLK1 | GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_WRTLOCK;
    while (!(GCLK->PCHCTRL[GCLK_DFLL48M_REF /*0*/].reg & GCLK_PCHCTRL_CHEN))
    { /* Wait for clock synchronization */
    }

    /* Set OSC16M to 16MHz */
    OSCCTRL->OSC16MCTRL.bit.FSEL = 3;
    OSCCTRL->OSC16MCTRL.bit.ONDEMAND = 0;
    OSCCTRL->OSC16MCTRL.bit.RUNSTDBY = 0;
    //gclk_setup(2, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC16M);
    //gclk_setup(1, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC16M);
    SystemCoreClock = 16000000ul;

    /* set DFLL at 32k */

    /* Remove the OnDemand mode, Workaround for errata 9905 */
    OSCCTRL->DFLLCTRL.bit.ONDEMAND = 0; 
    waitForDFLL();

    // 32kHz * 1500 = 48MHz CSTEP and FSTEP must be 50% or less ... 48005120
    OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_MUL(1465) | OSCCTRL_DFLLMUL_CSTEP(1) | OSCCTRL_DFLLMUL_FSTEP(1);
    waitForDFLL();

    OSCCTRL->DFLLCTRL.reg |= OSCCTRL_DFLLCTRL_MODE | OSCCTRL_DFLLCTRL_QLDIS | OSCCTRL_DFLLCTRL_ENABLE;
    waitForDFLL();

    /* Switch Generic Clock Generator 0 to 48MHz DFLL48M output. */
    gclk_setup(0, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_OE | GCLK_GENCTRL_IDC | GCLK_GENCTRL_DIV(1));

    for (int i = 0; i < 1000; i++)
    { // ??? lock ???
    }

    SystemCoreClock = 48000000ul;
}

void init_systick(void)
{
    // Set Systick to 1ms interval, common to all Cortex-M variants
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        while (1)
        { // Capture error
        }
    }
}
