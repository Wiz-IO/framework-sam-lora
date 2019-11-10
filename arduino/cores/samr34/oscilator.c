#include <stdint.h>
#include <samr34.h>

uint32_t SystemCoreClock = 4000000; // default RC

#define GCLK_DFLL48M_REF 0

static inline int gclk_is_syncing(const uint8_t generator)
{
    return (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(generator));
}

static void gclk_setup(int generator, uint32_t reg)
{
    while (gclk_is_syncing(generator))
    {
    }
    GCLK->GENCTRL[generator].reg = reg;
    while (gclk_is_syncing(generator))
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
    {
        /* Waiting performance level ready */
    }

    /* Software reset the GCLK module to ensure it is re-initialized correctly */
    GCLK->CTRLA.reg = GCLK_CTRLA_SWRST;
    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST)
    {
        /* Wait for synchronization */
    }

    /* set 32k - is enabled by default */
    gclk_setup(1, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K);
    GCLK->PCHCTRL[GCLK_DFLL48M_REF /*0*/].reg = GCLK_PCHCTRL_GEN_GCLK1 | GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_WRTLOCK;
    while (!(GCLK->PCHCTRL[GCLK_DFLL48M_REF /*0*/].reg & GCLK_PCHCTRL_CHEN))
    {
        /* Wait for clock synchronization */
    }

    /* set OSC16M to 16MHz */
    OSCCTRL->OSC16MCTRL.bit.FSEL = 3;
    OSCCTRL->OSC16MCTRL.bit.ONDEMAND = 0;
    OSCCTRL->OSC16MCTRL.bit.RUNSTDBY = 0;
    //gclk_setup(2, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC16M);
    //gclk_setup(0, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC16M);
    SystemCoreClock = 16000000ul;

    /* set DFLL at 32k */

    /* Workaround for errata 9905 */
    OSCCTRL->DFLLCTRL.reg &= ~OSCCTRL_DFLLCTRL_ONDEMAND;
    while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY))
    {
        /* Wait for synchronization */
    }

    // 32kHz * 1500 = 48MHz  CSTEP and FSTEP must be 50% or less
    OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_MUL(1465) | OSCCTRL_DFLLMUL_CSTEP(1) | OSCCTRL_DFLLMUL_FSTEP(1);
    while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY))
    {
        /* Wait for synchronization */
    }

    OSCCTRL->DFLLCTRL.reg |= OSCCTRL_DFLLCTRL_MODE | OSCCTRL_DFLLCTRL_QLDIS | OSCCTRL_DFLLCTRL_ENABLE;
    while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY))
    {
        /* Wait for synchronization */
    }

    gclk_setup(0, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_OE | GCLK_GENCTRL_IDC | GCLK_GENCTRL_DIV(1));

    for (int i = 0; i < 1000; i++)
    {
        __asm__ __volatile__(""); // ??? wait dfll lock ???
    }

    SystemCoreClock = 48000000ul;
}

void init_systick(void)
{
    // Set Systick to 1ms interval, common to all Cortex-M variants
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        while (1)
        {
            // Capture error
        }
        NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 2); /* set Priority for Systick Interrupt (2nd lowest) */
    }
}