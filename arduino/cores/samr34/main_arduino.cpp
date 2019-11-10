
#include "interface.h"

extern "C" void init_clock(void);
extern "C" void init_systick(void);

extern void initVariant(void) __attribute__((weak));
extern void setup(void);
extern void loop(void);

extern "C" int main(void)
{
    init_clock(); // 48 MHz
    init_systick();

    initVariant();
    setup();
    while (1)
    {
        loop();
        // if (serialEventRun) serialEventRun();
    }
    return 0;
}