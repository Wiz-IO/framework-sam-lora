/*
  SAMR3 - interface
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

#ifndef INTERFACE_H_
#define INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <samr3.h>

#define VARIANT_MCK (48000000ul)
  extern uint32_t SystemCoreClock;

#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

  long atol(const char *s);
  char *itoa(int value, char *result, int base);
  char *ltoa(long value, char *result, int base);
  char *utoa(unsigned value, char *result, int base);
  char *ultoa(unsigned long value, char *result, int base);
  double atof(register const char *s);

  uint32_t clockCyclesPerMicrosecond(void);
  uint32_t clockCyclesToMicroseconds(uint32_t a);
  uint32_t microsecondsToClockCycles(uint32_t a);

  unsigned int seconds(void);
  unsigned int millis(void);
  unsigned int micros(void);
  void delay(unsigned int);
  static __inline__ void delayMicroseconds(unsigned int) __attribute__((always_inline, unused));
  static __inline__ void delayMicroseconds(unsigned int usec)
  {
    if (usec == 0)
    {
      return;
    }
    /*
   *  The following loop:
   *
   *    for (; ul; ul--) {
   *      __asm__ volatile("");
   *    }
   *
   *  produce the following assembly code:
   *
   *    loop:
   *      subs r3, #1        // 1 Core cycle
   *      bne.n loop         // 1 Core cycle + 1 if branch is taken
   */
    // VARIANT_MCK / 1000000 == cycles needed to delay 1uS
    //                     3 == cycles used in a loop
    // Divide by 3 before multiplication with usec, so that the maximum usable usec value
    // with the D51 @ 120MHz is at least what it was when multipling by usec first at 48MHz.
    uint32_t n = usec * ((48000000u / 1000000) / 3);
    __asm__ __volatile__(
        "1:              \n"
        "   sub %0, #1   \n" // substract 1 from %0 (n)
        "   bne 1b       \n" // if result is not 0 jump to 1
        : "+r"(n)            // '%0' is n variable with RW constraints
        :                    // no input
        :                    // no clobber
    );
    // https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
    // https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Volatile
  }

#define DEBUG_ENABLE 1
  void putString(char *str);
#if DEBUG_ENABLE > 0
#define DBG_BUF_LEN 512
  extern char DBG_BUFFER[DBG_BUF_LEN];
#define DBG(FORMAT, ...)                        \
  {                                             \
    memset(DBG_BUFFER, 0, DBG_BUF_LEN);         \
    sprintf(DBG_BUFFER, FORMAT, ##__VA_ARGS__); \
    putString(DBG_BUFFER);                      \
  }

#else
#define DBG(FORMAT, ...)
#endif

#undef assert
#define assert(__e) ((__e) ? (void)0 : abort())

static inline int gclk_is_syncing(const uint8_t generator)
{
    return (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_GENCTRL(generator));
}
void gclk_setup(int generator, uint32_t reg);

void kick_watchdog(void);
uint32_t rnd(void);
uint32_t convert_byte_array_to_32_bit(uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif /* INTERFACE_H_ */