#include <stdint.h>
#include <samr34.h>
#include "cfg.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

int main(void);

void Dummy_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Dummy_Handler")));

__attribute__((section(".vectors")))
const DeviceVectors exception_table = {
    .pvStack = (void *)(&_estack),
    .pfnReset_Handler = (void *)Reset_Handler,
    .pfnNMI_Handler = (void *)NMI_Handler,
    .pfnHardFault_Handler = (void *)HardFault_Handler,
    .pvReservedM12 = (void *)(0UL), /* Reserved */
    .pvReservedM11 = (void *)(0UL), /* Reserved */
    .pvReservedM10 = (void *)(0UL), /* Reserved */
    .pvReservedM9 = (void *)(0UL),  /* Reserved */
    .pvReservedM8 = (void *)(0UL),  /* Reserved */
    .pvReservedM7 = (void *)(0UL),  /* Reserved */
    .pvReservedM6 = (void *)(0UL),  /* Reserved */
    .pfnSVC_Handler = (void *)SVC_Handler,
    .pvReservedM4 = (void *)(0UL), /* Reserved */
    .pvReservedM3 = (void *)(0UL), /* Reserved */
    .pfnPendSV_Handler = (void *)PendSV_Handler,
    .pfnSysTick_Handler = (void *)SysTick_Handler};

#define GCLK_DFLL48M_REF 0

static inline void loop_delay(uint32_t timeout)
{
    while (--timeout)
    {
    }
}

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

static void init_clock(void)
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
    ;
    while (!PM->INTFLAG.reg)
    {
    } /* Waiting performance level ready */

    /* Software reset the GCLK module to ensure it is re-initialized correctly */
    GCLK->CTRLA.reg = GCLK_CTRLA_SWRST;
    while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_SWRST)
    {
    }

    /* set 32k - is enabled by default */
    gclk_setup(1, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K);
    GCLK->PCHCTRL[GCLK_DFLL48M_REF /*0*/].reg = GCLK_PCHCTRL_GEN_GCLK1 | GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_WRTLOCK;
    while (!(GCLK->PCHCTRL[GCLK_DFLL48M_REF /*0*/].reg & GCLK_PCHCTRL_CHEN))
    {
    } /* Wait for clock synchronization */

    /* set OSC16M to 16MHz */
    OSCCTRL->OSC16MCTRL.bit.FSEL = 3;
    OSCCTRL->OSC16MCTRL.bit.ONDEMAND = 0;
    OSCCTRL->OSC16MCTRL.bit.RUNSTDBY = 0;
    //gclk_setup(2, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC16M);
    //gclk_setup(0, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSC16M);
    //SystemCoreClock = 16000000ul;

    //return;

    /* set DFLL at 32k */
    /* Workaround for errata 9905 */
    OSCCTRL->DFLLCTRL.reg &= ~OSCCTRL_DFLLCTRL_ONDEMAND;
    while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY))
    {
    } /* Wait for synchronization */

    // 32kHz * 1500 = 48MHz  CSTEP and FSTEP must be 50% or less
    OSCCTRL->DFLLMUL.reg = OSCCTRL_DFLLMUL_MUL(1465) | OSCCTRL_DFLLMUL_CSTEP(1) | OSCCTRL_DFLLMUL_FSTEP(1);
    while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY))
    {
    } /* Wait for synchronization */

    OSCCTRL->DFLLCTRL.reg |= OSCCTRL_DFLLCTRL_MODE | OSCCTRL_DFLLCTRL_QLDIS | OSCCTRL_DFLLCTRL_ENABLE;
    while (!(OSCCTRL->STATUS.reg & OSCCTRL_STATUS_DFLLRDY))
    {
    } /* Wait for synchronization */

    gclk_setup(0, GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_OE | GCLK_GENCTRL_IDC | GCLK_GENCTRL_DIV(1));

    //SystemCoreClock = 48000000ul;

    for (int i = 0; i < 1000; i++)
    {
    } // ??? wait dfll lock ???
}

/////////////////////////////////////////////////////////////////////////////////////////

void Reset_Handler(void)
{
    uint32_t *pSrc, *pDest;
    /* Initialize the relocate segment */
    pSrc = &_etext;
    pDest = &_srelocate;
    if (pSrc != pDest)
    {
        for (; pDest < &_erelocate;)
            *pDest++ = *pSrc++;
    }

    /* Clear the zero segment */
    for (pDest = &_szero; pDest < &_ezero;)
        *pDest++ = 0;

    /* Set the vector table base address */
    pSrc = (uint32_t *)&_sfixed;

    /* need vtor? */
    SCB->VTOR = ((uint32_t)pSrc & SCB_VTOR_TBLOFF_Msk);
    NVMCTRL->CTRLB.bit.MANW = 1;

    init_clock(); // 48 MHz

    //__libc_init_array();

    main();

    while (1)
    {
    }
}

void Dummy_Handler(void)
{
    while (1)
    {
    }
}

extern void *_sbrk(int incr)
{
    return (void *)-1;
}