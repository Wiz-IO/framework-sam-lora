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

#pragma once

#include <stdint.h>
#include "samr3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	GCLK_DFLL48M_REF        = 0, // DFLL48M Reference
	GCLK_DPLL               = 1, // FDPLL96M input clock source for reference
	GCLK_DPLL_32K           = 2, // FDPLL96M 32kHz clock for FDPLL96M internal lock timer
	GCLK_EIC                = 3, // EIC
	GCLK_USB                = 4, // USB
	GCLK_EVSYS_CHANNEL_0    = 5, // EVSYS_CHANNEL_0
	GCLK_EVSYS_CHANNEL_1    = 6, // EVSYS_CHANNEL_1
	GCLK_EVSYS_CHANNEL_2    = 7, // EVSYS_CHANNEL_2
	GCLK_EVSYS_CHANNEL_3    = 8, // EVSYS_CHANNEL_3
	GCLK_EVSYS_CHANNEL_4    = 9, // EVSYS_CHANNEL_4
	GCLK_EVSYS_CHANNEL_5    =10, // EVSYS_CHANNEL_5
	GCLK_EVSYS_CHANNEL_6    =11, // EVSYS_CHANNEL_6
	GCLK_EVSYS_CHANNEL_7    =12, // EVSYS_CHANNEL_7
	GCLK_EVSYS_CHANNEL_8    =13, // EVSYS_CHANNEL_8
	GCLK_EVSYS_CHANNEL_9    =14, // EVSYS_CHANNEL_9
	GCLK_EVSYS_CHANNEL_10   =15, // EVSYS_CHANNEL_10
	GCLK_EVSYS_CHANNEL_11   =16, // EVSYS_CHANNEL_11
	GCLK_SERCOM_01234_SLOW  =17, // SERCOM[0,1,2,3,4]_SLOW
	GCLK_SERCOM0_CORE       =18, // SERCOM0_CORE
	GCLK_SERCOM1_CORE       =19, // SERCOM1_CORE
	GCLK_SERCOM2_CORE       =20, // SERCOM2_CORE
	GCLK_SERCOM3_CORE       =21, // SERCOM3_CORE
	GCLK_SERCOM4_CORE       =22, // SERCOM4_CORE
	GCLK_SERCOM5_SLOW       =23, // SERCOM5_SLOW
	GCLK_SERCOM5_CORE       =24, // SERCOM5_CORE
	GCLK_TCC_01             =25, // TCC0,TCC1
	GCLK_TCC2               =26, // TCC2
	GCLK_TC_01              =27, // TC0,TC1
	GCLK_TC_23              =28, // TC2,TC3
	GCLK_TC4                =29, // TC4
	GCLK_ADC                =30, // ADC
	GCLK_AC                 =31, // AC
	GCLK_DAC                =32, // DAC
	GCLK_PTC                =33, // PTC
	GCLK_CCL                =34, // CCL
} gclk_map_e;


/* Definitions and types for pins */
typedef enum _EAnalogChannel
{
  No_ADC_Channel=-1,
  ADC_Channel0=0,
  ADC_Channel1=1,
  ADC_Channel2=2,
  ADC_Channel3=3,
  ADC_Channel4=4,
  ADC_Channel5=5,
  ADC_Channel6=6,
  ADC_Channel7=7,
  DAC_Channel0,
} EAnalogChannel ;

// Definitions for TC channels
typedef enum _ETCChannel
{
  NOT_ON_TIMER=-1,
  TCC0_CH0 = (0<<8)|(0),
  TCC0_CH1 = (0<<8)|(1),
  TCC0_CH2 = (0<<8)|(2),
  TCC0_CH3 = (0<<8)|(3),
  TCC0_CH4 = (0<<8)|(0), // Channel 4 is 0!
  TCC0_CH5 = (0<<8)|(1), // Channel 5 is 1!
  TCC0_CH6 = (0<<8)|(2), // Channel 6 is 2!
  TCC0_CH7 = (0<<8)|(3), // Channel 7 is 3!
  TCC1_CH0 = (1<<8)|(0),
  TCC1_CH1 = (1<<8)|(1),
  TCC1_CH2 = (1<<8)|(0), // Channel 2 is 0!
  TCC1_CH3 = (1<<8)|(1), // Channel 3 is 1!
  TCC2_CH0 = (2<<8)|(0),
  TCC2_CH1 = (2<<8)|(1),
  TCC2_CH2 = (2<<8)|(0), // Channel 2 is 0!
  TCC2_CH3 = (2<<8)|(1), // Channel 3 is 1!
  TC3_CH0  = (3<<8)|(0),
  TC3_CH1  = (3<<8)|(1),
  TC4_CH0  = (4<<8)|(0),
  TC4_CH1  = (4<<8)|(1),
  TC5_CH0  = (5<<8)|(0),
  TC5_CH1  = (5<<8)|(1),
} ETCChannel ;

extern const void* g_apTCInstances[TCC_INST_NUM+TC_INST_NUM] ;

#define GetTCNumber( x ) ( (x) >> 8 )
#define GetTCChannelNumber( x ) ( (x) & 0xff )
#define GetTC( x ) ( g_apTCInstances[(x) >> 8] )

// Definitions for PWM channels
typedef enum _EPWMChannel
{
  NOT_ON_PWM=-1,
  PWM0_CH0=TCC0_CH0,
  PWM0_CH1=TCC0_CH1,

  PWM1_CH0=TCC1_CH0,
  PWM1_CH1=TCC1_CH1,

  PWM2_CH0=TCC2_CH0,
  PWM2_CH1=TCC2_CH1,

  PWM3_CH0=TC3_CH0,
  PWM3_CH1=TC3_CH1,

  PWM4_CH0=TC4_CH0,
  PWM4_CH1=TC4_CH1,
  
  PWM5_CH0=TC5_CH0,
  PWM5_CH1=TC5_CH1,
} EPWMChannel ;

typedef enum _EPortType
{
  NOT_A_PORT=-1,
  PORTA=0,
  PORTB=1,
  PORTC=2,
} EPortType ;

typedef enum
{
  EXTERNAL_INT_0 = 0,
  EXTERNAL_INT_1,
  EXTERNAL_INT_2,
  EXTERNAL_INT_3,
  EXTERNAL_INT_4,
  EXTERNAL_INT_5,
  EXTERNAL_INT_6,
  EXTERNAL_INT_7,
  EXTERNAL_INT_8,
  EXTERNAL_INT_9,
  EXTERNAL_INT_10,
  EXTERNAL_INT_11,
  EXTERNAL_INT_12,
  EXTERNAL_INT_13,
  EXTERNAL_INT_14,
  EXTERNAL_INT_15,
  EXTERNAL_INT_NMI,
  EXTERNAL_NUM_INTERRUPTS,
  NOT_AN_INTERRUPT = -1,
  EXTERNAL_INT_NONE = NOT_AN_INTERRUPT,
} EExt_Interrupts ;

//A    B                 C       D          E      F   G   H
//EIC REF ADC AC PTC DAC SERCOM SERCOM_ALT TC/TCC TCC COM AC/GCLK

typedef enum _EPioType
{
  PIO_NOT_A_PIN=-1,     /* Not under control of a peripheral. */
  PIO_EXTINT=0,         /* The pin is controlled by the associated signal of peripheral A. */
  PIO_ANALOG,           /* The pin is controlled by the associated signal of peripheral B. */
  PIO_SERCOM,           /* The pin is controlled by the associated signal of peripheral C. */
  PIO_SERCOM_ALT,       /* The pin is controlled by the associated signal of peripheral D. */
  PIO_TIMER,            /* The pin is controlled by the associated signal of peripheral E. */
  PIO_TIMER_ALT,        /* The pin is controlled by the associated signal of peripheral F. */
  PIO_COM,              /* The pin is controlled by the associated signal of peripheral G. */
  PIO_AC_CLK,           /* The pin is controlled by the associated signal of peripheral H. */
  PIO_DIGITAL,          /* The pin is controlled by PORT. */
  PIO_INPUT,            /* The pin is controlled by PORT and is an input. */
  PIO_INPUT_PULLUP,     /* The pin is controlled by PORT and is an input with internal pull-up resistor enabled. */
  PIO_OUTPUT,           /* The pin is controlled by PORT and is an output. */

  PIO_PWM=PIO_TIMER,
  PIO_PWM_ALT=PIO_TIMER_ALT,
} EPioType ;

/**
 * Pin Attributes to be OR-ed
 */
#define PIN_ATTR_NONE          (0UL<<0)
#define PIN_ATTR_COMBO         (1UL<<0)
#define PIN_ATTR_ANALOG        (1UL<<1)
#define PIN_ATTR_DIGITAL       (1UL<<2)
#define PIN_ATTR_PWM           (1UL<<3)
#define PIN_ATTR_TIMER         (1UL<<4)
#define PIN_ATTR_TIMER_ALT     (1UL<<5)
#define PIN_ATTR_EXTINT        (1UL<<6)

/* Types used for the table below */
typedef struct _PinDescription
{
  EPortType       ulPort ;
  uint32_t        ulPin ;
  EPioType        ulPinType ;
  uint32_t        ulPinAttribute ;
  EAnalogChannel  ulADCChannelNumber ; /* ADC Channel number in the SAM device */
  EPWMChannel     ulPWMChannel ;
  ETCChannel      ulTCChannel ;
  EExt_Interrupts ulExtInt ;
} PinDescription ;

/* Pins table to be instantiated into variant.cpp */
extern const PinDescription g_APinDescription[] ;

/* Generic Clock Multiplexer IDs */
//TODO


#ifdef __cplusplus
} // extern "C"
#endif
