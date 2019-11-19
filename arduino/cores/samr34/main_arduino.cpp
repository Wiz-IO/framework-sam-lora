
#include "interface.h"

extern "C" void init_clock(void);
extern "C" void init_systick(void);

extern void serialEventRun(void) __attribute__((weak));
extern void initVariant(void) __attribute__((weak));
extern void setup(void);
extern void loop(void);

extern "C" int main(void)
{
    uint32_t *p = (uint32_t *)0;
    if (p[2] == 0x12345678) // from bootloader
    {
        SystemCoreClock = p[2]; // from bootloader
    }
    else
    {
        init_clock(); // 48 MHz
    }

    init_systick();
    initVariant();
    setup();
    while (1)
    {
        loop();
        if (serialEventRun)
            serialEventRun();
    }
    return 0;
}