/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef WVARIANT_H_
#define WVARIANT_H_

#include <variant.h>
#include <stdint.h>
#include "samr3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _ETCChannel
{
  TCC0_CH0 = (0<<4)|(0<<3)|(0),
  TCC0_CH1 = (0<<4)|(0<<3)|(1),
  TCC0_CH2 = (0<<4)|(0<<3)|(2),
  TCC0_CH3 = (0<<4)|(0<<3)|(3),
  TCC0_CH4 = (0<<4)|(0<<3)|(0), // Channel 4 is 0!
  TCC0_CH5 = (0<<4)|(0<<3)|(1), // Channel 5 is 1!
  TCC0_CH6 = (0<<4)|(0<<3)|(2), // Channel 6 is 2!
  TCC0_CH7 = (0<<4)|(0<<3)|(3), // Channel 7 is 3!

  TCC1_CH0 = (1<<4)|(0<<3)|(0),
  TCC1_CH1 = (1<<4)|(0<<3)|(1),
  TCC1_CH2 = (1<<4)|(0<<3)|(0), // Channel 2 is 0!
  TCC1_CH3 = (1<<4)|(0<<3)|(1), // Channel 3 is 1!

  TCC2_CH0 = (2<<4)|(0<<3)|(0),
  TCC2_CH1 = (2<<4)|(0<<3)|(1),

  TC0_CH0  = (0<<4)|(1<<3)|(0),
  TC0_CH1  = (0<<4)|(1<<3)|(1),
  TC1_CH0  = (1<<4)|(1<<3)|(0),
  TC1_CH1  = (1<<4)|(1<<3)|(1),
  TC2_CH0  = (2<<4)|(1<<3)|(0),
  TC2_CH1  = (2<<4)|(1<<3)|(1),
  TC3_CH0  = (3<<4)|(1<<3)|(0),
  TC3_CH1  = (3<<4)|(1<<3)|(1),
  TC4_CH0  = (4<<4)|(1<<3)|(0),
  TC4_CH1  = (4<<4)|(1<<3)|(1),
  TC5_CH0  = (5<<4)|(1<<3)|(0),
  TC5_CH1  = (5<<4)|(1<<3)|(1),
  TC6_CH0  = (6<<4)|(1<<3)|(0),
  TC6_CH1  = (6<<4)|(1<<3)|(1),
  TC7_CH0  = (7<<4)|(1<<3)|(0),
  TC7_CH1  = (7<<4)|(1<<3)|(1),

  TIMER_NONE = (1<<7),
} ETCChannel ;

extern const void* g_apTCInstances[TCC_INST_NUM + TC_INST_NUM] ;

#define GetTCNumber( x )        ( ((x) >> 4) & 0x07 )
#define GetTCType( x )          ( ((x) >> 3) & 0x01 )
#define GetTCChannelNumber( x ) ( (x) & 0x07 )

#define GetTC( x ) ( GetTCType(x) == 0 ? g_apTCInstances[GetTCNumber(x)] : (GetTCNumber(x) == 4 ? TC4 : g_apTCInstances[GetTCNumber(x) + TCC_INST_NUM]) )

// TODO: Definitions for GCLK_CCL column, AC. The format may change.
// GCLK/CCL Enable (1 bit: 0=enabled, 1=disabled) | GCLK (3 bits: 0-7) | CCL (4 bits: 2 for CCL number, 2 for pin (or 4 for IN/OUT pin ID on D51))
typedef enum _EGCLK_CCL
{
  GCLK_CCL_NONE = (1<<7),
} EGCLK_CCL ;

typedef enum _EPortType
{
  PORTA   = 0,
  PORTB   = 1,
  PORTC   = 2,
  PORTD   = 3,
  NOT_A_PORT,
} EPortType ;

#define GetPort( x ) g_APinDescription[x].ulPort
#define GetPin( x ) g_APinDescription[x].ulPin

/* Definitions and types for pins */
typedef enum _EAnalogChannel
{
  ADC_0 = 0,
  ADC_1,
  ADC_2,
  ADC_3,
  ADC_4,
  ADC_5,
  ADC_6,
  ADC_7,
  ADC_8,
  ADC_9,
  ADC_10,
  ADC_11,
  ADC_12,
  ADC_13,
  ADC_14,
  ADC_15,
  ADC_16,
  ADC_17,
  ADC_18,
  ADC_19,
  
  DAC_0,
  DAC_1,

  ADC_NONE,
} EAnalogChannel ;

typedef enum
{
  EINT_0 = 0,
  EINT_1,
  EINT_2,
  EINT_3,
  EINT_4,
  EINT_5,
  EINT_6,
  EINT_7,
  EINT_8,
  EINT_9,
  EINT_10,
  EINT_11,
  EINT_12,
  EINT_13,
  EINT_14,
  EINT_15,
  EINT_NMI,
  EXTERNAL_NUM_INTERRUPTS,
  NOT_AN_INTERRUPT,
  EINT_NONE = NOT_AN_INTERRUPT,
} EExt_Interrupts;

#define GetExtInt( x ) g_APinDescription[x].ulExtInt
#define GetADC( x ) g_APinDescription[x].ulADCChannelNumber

/* Copied from wiring_constants.h */
#define INPUT           (0x0)
#define OUTPUT          (0x1)
#define INPUT_PULLUP    (0x2)
#define INPUT_PULLDOWN  (0x3)

typedef enum _EPioType
{
  PIO_INPUT = INPUT,                    /* The pin is controlled by PORT and is an input. */
  PIO_OUTPUT = OUTPUT,                  /* The pin is controlled by PORT and is an output. */
  PIO_INPUT_PULLUP = INPUT_PULLUP,      /* The pin is controlled by PORT and is an input with internal pull-up resistor enabled. */
  PIO_INPUT_PULLDOWN = INPUT_PULLDOWN,  /* The pin is controlled by PORT and is an input with internal pull-down resistor enabled. */

  PIO_EXTINT                      =  4, /* The pin is controlled by the EXTINT peripheral and is an input with interrupt. */
  PIO_ANALOG_ADC                  =  5, /* The pin is controlled by the ANALOG peripheral and is an ADC input. */
  PIO_ANALOG_DAC                  =  6, /* The pin is controlled by the ANALOG peripheral and is a DAC output. */
  PIO_ANALOG_REF                  =  7, /* The pin is controlled by the ANALOG peripheral and is a voltage reference input (3.3V MAX). */
  PIO_ANALOG_AC                   =  8, /* The pin is controlled by the ANALOG peripheral and is used by the AC (analog comparator). */
  PIO_ANALOG_PTC                  =  9, /* The pin is controlled by the ANALOG peripheral and is used by the PTC (peripheral touch controller). */
  PIO_ANALOG_SDADC                = 10, /* The pin is controlled by the ANALOG peripheral and is used by the SDADC (sigma-delta ADC). */
  PIO_ANALOG_OPAMP                = 11, /* The pin is controlled by the ANALOG peripheral and is used by the OPAMP (L21 only). */
  PIO_TIMER_PWM                   = 12, /* The pin is controlled by a TIMER peripheral and is a PWM output. */
  PIO_TIMER_CAPTURE               = 13, /* The pin is controlled by a TIMER peripheral and is a capture input. */
  PIO_SERCOM                      = 14, /* The pin is controlled by a SERCOM peripheral (UART, SPI, or I2C). */
  PIO_COM                         = 15, /* The pin is controlled by the COM peripheral (USB, CAN, or CORTEX (I2S on D21 only)). */
  PIO_USB                         = 16, /* The pin is controlled by the COM peripheral (except C21). */
  PIO_CAN                         = 17, /* The pin is controlled by the COM or SDHC peripheral (D51) or the COM peripheral (C21). */
  PIO_QSPI                        = 18, /* The pin is controlled by the COM peripheral (QSPI on D51 only). */
  PIO_SDHC                        = 19, /* The pin is controlled by the SDHC peripheral (SDHC and CAN0 on D51 only). */
  PIO_I2S                         = 20, /* The pin is controlled by the I2S (inter-IC sound) peripheral (D51) or the COM peripheral (D21). */
  PIO_GMAC                        = 21, /* The pin is controlled by the GMAC peripheral (ethernet on D51 only). */
  PIO_PCC                         = 22, /* The pin is controlled by the PCC peripheral (parallel capture controller on D51 only). */
  PIO_AC_GCLK                     = 23, /* The pin is controlled by the AC_GCLK peripheral (analog comparator / generic clock). */
  PIO_CCL                         = 24, /* The pin is controlled by the CCL (configurable custom logic) peripheral (I/O). */

  PIO_SERCOM_RF                   = 25,

  PIO_MULTI,        /* The pin can be configured to any type based on the attributes. */
  PIO_STARTUP,      /* Used as parameter to pinPeripheral() only to set startup state (enable INEN only) */
  PIO_NOT_A_PIN,    /* Not under control of a peripheral. */

  PIO_NONE = -1
} EPioType ;

/* Pin Attributes to be OR-ed */
#define PIN_ATTR_NONE              (0UL << 0)

#define PIN_ATTR_INPUT             (1UL << PIO_INPUT)
#define PIN_ATTR_OUTPUT            (1UL << PIO_OUTPUT)
#define PIN_ATTR_INPUT_PULLUP      (1UL << PIO_INPUT_PULLUP)
#define PIN_ATTR_INPUT_PULLDOWN    (1UL << PIO_INPUT_PULLDOWN)
#define PIN_ATTR_DIGITAL           (PIN_ATTR_INPUT|PIN_ATTR_INPUT_PULLUP|PIN_ATTR_INPUT_PULLDOWN|PIN_ATTR_OUTPUT)

#define PIN_ATTR_EXTINT            (1UL << PIO_EXTINT)

#define PIN_ATTR_ADC               (1UL << PIO_ANALOG_ADC)
#define PIN_ATTR_DAC               (1UL << PIO_ANALOG_DAC)
#define PIN_ATTR_REF               (1UL << PIO_ANALOG_REF)
#define PIN_ATTR_AC                (1UL << PIO_ANALOG_AC)
#define PIN_ATTR_PTC               (1UL << PIO_ANALOG_PTC)
#define PIN_ATTR_SDADC             (1UL << PIO_ANALOG_SDADC)
#define PIN_ATTR_OPAMP             (1UL << PIO_ANALOG_OPAMP)
#define PIN_ATTR_ANALOG            PIN_ATTR_ADC

#define PIN_ATTR_TIMER_PWM         (1UL << PIO_TIMER_PWM)
#define PIN_ATTR_TIMER_CAPTURE     (1UL << PIO_TIMER_CAPTURE)
#define PIN_ATTR_TIMER_BOTH        (PIN_ATTR_TIMER_PWM|PIN_ATTR_TIMER_CAPTURE)
#define PIN_ATTR_TIMER             PIN_ATTR_TIMER_BOTH

#define PIN_ATTR_SERCOM            (1UL << PIO_SERCOM)
#define PIN_ATTR_COM               (1UL << PIO_COM)
#define PIN_ATTR_USB               (1UL << PIO_USB)
#define PIN_ATTR_CAN               (1UL << PIO_CAN)
#define PIN_ATTR_QSPI              (1UL << PIO_QSPI)
#define PIN_ATTR_SDHC              (1UL << PIO_SDHC)
#define PIN_ATTR_I2S               (1UL << PIO_I2S)
#define PIN_ATTR_GMAC              (1UL << PIO_GMAC)

#define PIN_ATTR_PCC               (1UL << PIO_PCC)
#define PIN_ATTR_AC_GCLK           (1UL << PIO_AC_GCLK)
#define PIN_ATTR_CCL               (1UL << PIO_CCL)

/*            A  |------ B -------|   C        D        E     F     G         H       I  
D21/L21/C21: EIC REF ADC AC PTC DAC SERCOM SERCOM_ALT TC/TCC TCC   COM     AC/GCLK   CCL
*/
typedef enum _EPioPeripheral
{
	PER_EXTINT        = 0, /* The pin is controlled by the associated signal of peripheral A. */
	PER_ANALOG        = 1, /* The pin is controlled by the associated signal of peripheral B. */
	PER_SERCOM        = 2, /* The pin is controlled by the associated signal of peripheral C. */
	PER_SERCOM_ALT    = 3, /* The pin is controlled by the associated signal of peripheral D. */
	PER_TIMER         = 4, /* The pin is controlled by the associated signal of peripheral E. */
  PER_TIMER_ALT     = 5, /* The pin is controlled by the associated signal of peripheral F. */
	PER_COM           = 6, /* The pin is controlled by the associated signal of peripheral G. */
	PER_AC_CLK        = 7, /* The pin is controlled by the associated signal of peripheral H. */
	PER_CCL           = 8, /* The pin is controlled by the associated signal of peripheral I. */
  PER_PORT,              /* The pin is controlled by PORT. */
} EPioPeripheral ;

/* Peripheral Attributes to be OR-ed */
#define PER_ATTR_NONE                             (0UL<<0)

#define PER_ATTR_SERCOM_STD                       (0UL<<0)
#define PER_ATTR_SERCOM_ALT                       (1UL<<0)
#define PER_ATTR_SERCOM_MASK                      (1UL<<0)

#define PER_ATTR_TIMER_STD                        (0UL<<1)
#define PER_ATTR_TIMER_ALT                        (1UL<<1)
#define PER_ATTR_TIMER_ALT2                       (2UL<<1)
#define PER_ATTR_TIMER_MASK                       (3UL<<1)

#define PER_ATTR_DRIVE_STD                        (0UL<<3)
#define PER_ATTR_DRIVE_STRONG                     (1UL<<3)
#define PER_ATTR_DRIVE_MASK                       (1UL<<3)

#define PER_ATTR_OUTPUT_TYPE_STD                  (0UL<<4)
#define PER_ATTR_OUTPUT_TYPE_OPEN_DRAIN           (1UL<<4)
#define PER_ATTR_OUTPUT_TYPE_OPEN_SOURCE          (2UL<<4)
#define PER_ATTR_OUTPUT_TYPE_BUSKEEPER            (3UL<<4)
#define PER_ATTR_OUTPUT_TYPE_MASK                 (3UL<<4)

#define PER_ATTR_INPUT_SYNCHRONIZER_ON_DEMAND     (0UL<<6)
#define PER_ATTR_INPUT_SYNCHRONIZER_ALWAYS_ON     (1UL<<6)
#define PER_ATTR_INPUT_SYNCHRONIZER_MASK          (1UL<<6)

#define PER_ATTR_ADC_STD                          (0UL<<7)
#define PER_ATTR_ADC_ALT                          (1UL<<7)
#define PER_ATTR_ADC_MASK                         (1UL<<7)


/* PinDescription table */
// This struct MUST be 12 bytes long (elements are ordered to prevent unaligned access).
typedef struct _PinDescription
{
  uint8_t         ulPort ;	                  // Must be 8 bits
  uint8_t         ulPin ;	                    // Must be 8 bits
  uint8_t         ulPinType ;	                // Must be 8 bits
  uint8_t         ulPeripheralAttribute ;	    // Must be 8 bit bitfield
  uint32_t        ulPinAttribute ;	          // Must be 32 bit bitfield
  uint8_t         ulTCChannel ;	              // Must be 8 bits
  uint8_t         ulADCChannelNumber ;	      // Must be 8 bits
  uint8_t         ulExtInt ;	                // Must be 8 bits
  uint8_t         ulGCLKCCL ;	                // Must be 8 bits
} PinDescription ;

/* Pins table to be instantiated into variant.cpp */
extern const PinDescription g_APinDescription[] ;

/* Generic Clock Multiplexer IDs */
#define GCM_DFLL48M_REF           (0x00U)
#define GCM_FDPLL96M_INPUT        (0x01U)
#define GCM_FDPLL96M_32K          (0x02U)
#define GCM_EIC                   (0x03U)
#define GCM_USB                   (0x04U)
#define GCM_EVSYS_CHANNEL_0       (0x05U)
#define GCM_EVSYS_CHANNEL_1       (0x06U)
#define GCM_EVSYS_CHANNEL_2       (0x07U)
#define GCM_EVSYS_CHANNEL_3       (0x08U)
#define GCM_EVSYS_CHANNEL_4       (0x09U)
#define GCM_EVSYS_CHANNEL_5       (0x0AU)
#define GCM_EVSYS_CHANNEL_6       (0x0BU)
#define GCM_EVSYS_CHANNEL_7       (0x0CU)
#define GCM_EVSYS_CHANNEL_8       (0x0DU)
#define GCM_EVSYS_CHANNEL_9       (0x0EU)
#define GCM_EVSYS_CHANNEL_10      (0x0FU)
#define GCM_EVSYS_CHANNEL_11      (0x10U)
#define GCM_SERCOMx_SLOW          (0x11U)
#define GCM_SERCOM0_CORE          (0x12U)
#define GCM_SERCOM1_CORE          (0x13U)
#define GCM_SERCOM2_CORE          (0x14U)
#define GCM_SERCOM3_CORE          (0x15U)
#define GCM_SERCOM4_CORE          (0x16U)
#define GCM_SERCOM5_SLOW          (0x17U)
#define GCM_SERCOM5_CORE          (0x18U)
#define GCM_TCC0_TCC1             (0x19U)
#define GCM_TCC2                  (0x1AU)
#define GCM_TC0_TC1               (0x1BU)
#define GCM_TC2_TC3               (0x1CU)
#define GCM_TC4                   (0x1DU)
#define GCM_ADC                   (0x1EU)
#define GCM_AC                    (0x1FU)
#define GCM_DAC                   (0x20U)
#define GCM_PTC                   (0x21U)
#define GCM_CCL                   (0x22U)


int pinPeripheral(uint32_t ulPin, uint32_t ulPeripheral);

#ifdef __cplusplus
} // extern "C"
#endif

#endif