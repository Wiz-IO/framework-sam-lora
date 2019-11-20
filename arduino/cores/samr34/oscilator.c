#include <interface.h>

uint32_t SystemCoreClock = VARIANT_MCK; // 48000000ul;

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

static void enable_XOSC32K(void)
{
    /* Enable external XOSC32K oscilator */
    OSC32KCTRL->XOSC32K.reg = (OSC32KCTRL_XOSC32K_STARTUP(0) |
                               OSC32KCTRL_XOSC32K_XTALEN |
                               OSC32KCTRL_XOSC32K_EN32K |
                               OSC32KCTRL_XOSC32K_EN1K);
    OSC32KCTRL->XOSC32K.bit.ENABLE = 1;
    while (0 == (OSC32KCTRL->STATUS.reg & OSC32KCTRL_STATUS_XOSC32KRDY))
    {
    }
}

void init_system_clock(void)
{
    enable_XOSC32K();

    /* Software reset the GCLK module to ensure it is re-initialized correctly */
    GCLK->CTRLA.reg = GCLK_CTRLA_SWRST;
    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST)
    {
    }

    /* Switch GCLK0 to XOSC32K */
    gclk_setup(0, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_XOSC32K); // GCLK_GENCTRL_SRC_OSCULP32K

    GCLK->PCHCTRL[0 /*GCLK_DFLL48M_REF*/].reg = GCLK_PCHCTRL_GEN_GCLK0 |
                                                GCLK_PCHCTRL_CHEN |
                                                GCLK_PCHCTRL_WRTLOCK;
    while (0 == (GCLK->PCHCTRL[0 /*GCLK_DFLL48M_REF*/].reg & GCLK_PCHCTRL_CHEN))
    {
    }

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

    /* Switch GCLK0 to DFLL48M output. */
    gclk_setup(0, GCLK_GENCTRL_DIV(1) |
                      GCLK_GENCTRL_SRC_DFLL48M |
                      GCLK_GENCTRL_IDC |
                      GCLK_GENCTRL_OE |
                      GCLK_GENCTRL_GENEN);

    for (int i = 0; i < 1000; i++)
    { // ??? stabilize ???
    }

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