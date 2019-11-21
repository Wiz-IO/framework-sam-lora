
#include "interface.h"

void serialEventRun(void) __attribute__((weak));
void initVariant(void) __attribute__((weak));
void setup(void);
void loop(void);

extern "C" void SYS_TimerTaskHandler(void) __attribute__((weak));

extern "C" int main(void)
{
    initVariant();
    setup();
    while (1)
    {
        loop();

        if (serialEventRun)
            serialEventRun();

        if (SYS_TimerTaskHandler)
            SYS_TimerTaskHandler();

#ifndef DISABLE_WATCHDOG
        kick_watchdog();
#endif
    }
    return 0;
}