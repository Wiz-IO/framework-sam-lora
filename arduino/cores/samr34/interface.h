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

  extern uint32_t SystemCoreClock;

#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

  void digitalToggle(uint32_t ulPin, uint32_t pause);

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
  void gclk_channel_setup(const uint8_t channel, uint32_t val);
  void gclk_channel_enable(const uint8_t channel);
  void gclk_channel_disable(const uint8_t channel);

  enum sleep_mode_e
  {
    /** IDLE sleep mode */
    SLEEP_MODE_IDLE = PM_SLEEPCFG_SLEEPMODE(0x2),
    /** STANDBY sleep mode */
    SLEEP_MODE_STANDBY = PM_SLEEPCFG_SLEEPMODE_STANDBY,
    /** BACKUP sleep mode */
    SLEEP_MODE_BACKUP = PM_SLEEPCFG_SLEEPMODE_BACKUP,
    /** OFF sleep mode */
    SLEEP_MODE_OFF = PM_SLEEPCFG_SLEEPMODE_OFF,
  };

  static inline void sys_set_sleep_mode(const enum sleep_mode_e sleep_mode)
  {
    PM->SLEEPCFG.reg = sleep_mode;
    while (PM->SLEEPCFG.reg != sleep_mode)
    {
    }
  }

  static inline void sys_sleep(void) /* DO NOT ENTER FAST ! */
  {
    __DSB();
    __WFI();
  }

  enum reset_cause_e
  {
    /** The system was last reset by a backup reset */
    RESET_CAUSE_BACKUP = RSTC_RCAUSE_BACKUP,
    /** The system was last reset by a software reset */
    RESET_CAUSE_SOFTWARE = RSTC_RCAUSE_SYST,
    /** The system was last reset by the watchdog timer */
    RESET_CAUSE_WDT = RSTC_RCAUSE_WDT,
    /** The system was last reset because the external reset line was pulled low */
    RESET_CAUSE_EXTERNAL_RESET = RSTC_RCAUSE_EXT,
#if SAML21 || SAMR30 || (SAMR34)
    /** The system was last reset by the BOD33. */
    RESET_CAUSE_BOD33 = RSTC_RCAUSE_BOD33,
    /** The system was last reset by the BOD12 */
    RESET_CAUSE_BOD12 = RSTC_RCAUSE_BOD12,
#else
  /** The system was last reset by the BOD VDD. */
  RESET_CAUSE_BOD33 = RSTC_RCAUSE_BODVDD,
  /** The system was last reset by the BOD CORE. */
  RESET_CAUSE_BOD12 = RSTC_RCAUSE_BODCORE,
#endif
    /** The system was last reset by the POR (Power on reset). */
    RESET_CAUSE_POR = RSTC_RCAUSE_POR,
  };

  static inline enum reset_cause_e get_reset_cause(void)
  {
    return (enum reset_cause_e)RSTC->RCAUSE.reg;
  }

  void kick_watchdog(void);

  uint32_t rnd(void);

  uint32_t convert_byte_array_to_32_bit(uint8_t *data);

  int serial_number(uint8_t *b, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* INTERFACE_H_ */