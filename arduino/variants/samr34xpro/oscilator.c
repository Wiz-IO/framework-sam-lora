#include "variant.h"
#include <interface.h>

uint32_t SystemCoreClock = 4000000; // default freq;

static void waitForDFLL(void)
{
    while ((OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY) == 0)
    {
    }
}

static void enable_XOSC32K(void)
{
    /* Enable external XOSC32K oscilator */
    OSC32KCTRL->XOSC32K.reg = (OSC32KCTRL_XOSC32K_ENABLE |
                               OSC32KCTRL_XOSC32K_STARTUP(0) |
                               OSC32KCTRL_XOSC32K_XTALEN |
                               OSC32KCTRL_XOSC32K_EN32K |
                               OSC32KCTRL_XOSC32K_EN1K); // for RTC
    while (0 == (OSC32KCTRL->STATUS.reg & OSC32KCTRL_STATUS_XOSC32KRDY))
    {
    }
}

/*  from XOSC32K to DFLL48M  */
void init_system_clock(void)
{
    /* Turn on the digital interface clock: IS ON */
    //system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBA, MCLK_APBAMASK_GCLK);

    /* Software reset the GCLK module to ensure it is re-initialized correctly */
    GCLK->CTRLA.reg = GCLK_CTRLA_SWRST;
    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST)
    {
    }

    /* CPU and BUS clocks */
    MCLK->BUPDIV.reg = MCLK_BUPDIV_BUPDIV(1 << 0);
    MCLK->LPDIV.reg = MCLK_LPDIV_LPDIV(1 << 0);
    MCLK->CPUDIV.reg = MCLK_CPUDIV_CPUDIV(1 << 0);

    enable_XOSC32K();

    /* SWITCH GCLK1 TO XOSC32K */
    gclk_setup(1, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_XOSC32K);

    /* GCLK_DFLL48M_REF */
    gclk_channel_setup(0, GCLK_PCHCTRL_GEN_GCLK1 | GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_WRTLOCK); // SET TO GCLK1

    /* Remove the OnDemand mode, Workaround for errata 9905 */
    OSCCTRL->DFLLCTRL.bit.ONDEMAND = 0;
    waitForDFLL();

    /* 32kHz * 1500 = 48MHz CSTEP and FSTEP must be 50% or less */
    OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_MUL(1465) | /* 48005120 Hz */
                           OSCCTRL_DFLLMUL_CSTEP(1) |
                           OSCCTRL_DFLLMUL_FSTEP(1);
    waitForDFLL();

    OSCCTRL->DFLLCTRL.reg |= OSCCTRL_DFLLCTRL_MODE |
                             OSCCTRL_DFLLCTRL_QLDIS |
                             OSCCTRL_DFLLCTRL_ENABLE;
    waitForDFLL();

    // Wait for the fine and coarse locks.
    while (!OSCCTRL->INTFLAG.bit.DFLLLCKC && !OSCCTRL->INTFLAG.bit.DFLLLCKF)
    {
    }

    // Wait until the DFLL is ready.
    while (!OSCCTRL->INTFLAG.bit.DFLLRDY)
    {
    }

    /* SWITCH MAIN GCLK0 to DFLL48M output. */
    gclk_setup(0, GCLK_GENCTRL_GENEN |
                      GCLK_GENCTRL_SRC_DFLL48M |
                      GCLK_GENCTRL_DIV(0) |
                      GCLK_GENCTRL_IDC);

    SystemCoreClock = VARIANT_MCK; // 48000000ul;
}

void init_systick(void)
{
    /* Set Systick to 1ms interval, common to all Cortex-M variants */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        while (1)
        { // Capture error
        }
    }
}

/** Tick Counter united by ms */
volatile uint32_t _ulTickCount = 0;

void SysTick_Handler(void)
{
    _ulTickCount++; // Increment tick count each ms
}