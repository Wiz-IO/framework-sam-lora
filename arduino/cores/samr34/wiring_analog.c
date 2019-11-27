/*
  Copyright (c) 2014 Arduino LLC.  All right reserved.
  Copyright (c) 2017 MattairTech LLC. All right reserved.

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

#include <Arduino.h>
#include <wiring_private.h>
#include <WVariant.h>

#ifdef __cplusplus
extern "C"
{
#endif

// not defined for SAML or SAMC in version of CMSIS used
#ifndef ADC_INPUTCTRL_MUXNEG_GND
#define ADC_INPUTCTRL_MUXNEG_GND (0x18ul << ADC_INPUTCTRL_MUXNEG_Pos)
#endif

    /* Mapping of timer numbers (array index) to generic clock IDs GCM_* values are defined in WVariant.h in the core. */
    const uint8_t timerClockIDs[] = {
        GCM_TCC0_TCC1,
        GCM_TCC0_TCC1,
        GCM_TCC2,
        GCM_TC0_TC1,
        GCM_TC0_TC1,
        GCM_TC2_TC3,
        GCM_TC2_TC3,
        GCM_TC4,
    };

#if (defined(TIMER_732Hz) | defined(TIMER_366Hz) | defined(TIMER_244Hz) | defined(TIMER_183Hz) | defined(TIMER_146Hz) | defined(TIMER_122Hz) | defined(TIMER_105Hz) | defined(TIMER_81Hz) | defined(TIMER_61Hz) | defined(TIMER_31Hz) | (defined(TIMER_1465Hz) && SAMD51))
#define TIMER_RESOLUTION_IS_16BIT
#else
#define TIMER_RESOLUTION_IS_8BIT
#endif

    static int _writeResolution = 8;
    static int _readResolution = 10;
    static int _ADCResolution = 10;
    uint8_t ADCinitialized = 0;
    uint8_t DACinitialized = 0;
    uint8_t REFinitialized = 0;
    extern bool dacEnabled[];

    static __inline__ void syncADC() __attribute__((always_inline, unused));
    static void syncADC()
    {
        while (ADC->SYNCBUSY.reg & ADC_SYNCBUSY_MASK)
        {
        }
    }

    static __inline__ void syncDAC() __attribute__((always_inline, unused));
    static void syncDAC()
    {
        while (DAC->SYNCBUSY.reg & DAC_SYNCBUSY_MASK)
        {
        }
    }

    static __inline__ void syncTC_16(Tc *TCx) __attribute__((always_inline, unused));
    static void syncTC_16(Tc *TCx)
    {
        while (TCx->COUNT16.SYNCBUSY.reg & (TC_SYNCBUSY_SWRST | TC_SYNCBUSY_ENABLE | TC_SYNCBUSY_CTRLB | TC_SYNCBUSY_STATUS | TC_SYNCBUSY_COUNT))
        {
        }
    }

    static __inline__ void syncTCC(Tcc *TCCx) __attribute__((always_inline, unused));
    static void syncTCC(Tcc *TCCx)
    {
        while (TCCx->SYNCBUSY.reg & TCC_SYNCBUSY_MASK)
        {
        }
    }

    /* Initialize Analog Controller */
    void initADC(void)
    {
        uint32_t biasrefbuf = (*((uint32_t *)ADC_FUSES_BIASREFBUF_ADDR) & ADC_FUSES_BIASREFBUF_Msk) >> ADC_FUSES_BIASREFBUF_Pos;
        uint32_t biascomp = (*((uint32_t *)ADC_FUSES_BIASCOMP_ADDR) & ADC_FUSES_BIASCOMP_Msk) >> ADC_FUSES_BIASCOMP_Pos;
        ADC->CALIB.reg = ADC_CALIB_BIASREFBUF(biasrefbuf) | ADC_CALIB_BIASCOMP(biascomp);
        syncADC();

        // Setting clock, prescaler and resolution
        SUPC->VREF.reg |= SUPC_VREF_VREFOE; // Enable Supply Controller Reference output for use with ADC and DAC (AR_INTREF)
        GCLK->PCHCTRL[GCM_ADC].reg = (GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK0);
        while ((GCLK->PCHCTRL[GCM_ADC].reg & GCLK_PCHCTRL_CHEN) != GCLK_PCHCTRL_CHEN)
        { // wait for sync
        }
        ADC->CTRLB.reg = ADC_CTRLB_PRESCALER_DIV256; // Divide Clock by 256.
        ADC->CTRLC.reg = ADC_CTRLC_RESSEL_10BIT;     // 10 bits resolution as default
        syncADC();

        // Setting configuration
        ADC->SAMPCTRL.reg = 0x3f; // Set max Sampling Time Length
        syncADC();
        ADC->INPUTCTRL.reg = ADC_INPUTCTRL_MUXNEG_GND; // No Negative input (Internal Ground)
        syncADC();
        // Averaging (see datasheet table in AVGCTRL register description)
        ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1 |  // 1 sample only (no oversampling nor averaging)
                           ADC_AVGCTRL_ADJRES(0x0ul); // Adjusting result by 0
        syncADC();
        ADCinitialized = 1;
        if (!REFinitialized)
        {
            analogReference(VARIANT_AR_DEFAULT); // Use default reference from variant.h
        }
    }

    /* Initialize DAC */
    void initDAC(void)
    {
        while (GCLK->SYNCBUSY.reg & GCLK_SYNCBUSY_MASK)
        {
        }
        GCLK->PCHCTRL[GCM_DAC].reg = (GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK0);
        while ((GCLK->PCHCTRL[GCM_DAC].reg & GCLK_PCHCTRL_CHEN) == 0)
        {
        }
        while (DAC->SYNCBUSY.reg & DAC_SYNCBUSY_MASK)
        {
        }
        DAC->CTRLB.reg = DAC_CTRLB_REFSEL_VDDANA;
        DAC->DACCTRL[0].reg = (DAC_DACCTRL_REFRESH(3) | DAC_DACCTRL_CCTRL(2));
        DAC->DACCTRL[1].reg = (DAC_DACCTRL_REFRESH(3) | DAC_DACCTRL_CCTRL(2));
        DACinitialized = 1;
    }

    void analogReadResolution(int res)
    {
        if (!ADCinitialized)
        {
            initADC();
        }
        _readResolution = res;
        if (res > 10)
        {
            ADC->CTRLC.bit.RESSEL = ADC_CTRLC_RESSEL_12BIT_Val;
            _ADCResolution = 12;
        }
        else if (res > 8)
        {
            ADC->CTRLC.bit.RESSEL = ADC_CTRLC_RESSEL_10BIT_Val;
            _ADCResolution = 10;
        }
        else
        {
            ADC->CTRLC.bit.RESSEL = ADC_CTRLC_RESSEL_8BIT_Val;
            _ADCResolution = 8;
        }
        syncADC();
    }

    void analogWriteResolution(int res)
    {
        _writeResolution = res;
    }

    static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to)
    {
        if (from == to)
        {
            return value;
        }
        if (from > to)
        {
            return value >> (from - to);
        }
        return value << (to - from);
    }

    void analogReference(eAnalogReference mode)
    {
        if (!ADCinitialized)
        {
            REFinitialized = 1; // to prevent re-entry
            initADC();
        }

#if defined(REFA_PIN)
        if (mode == AR_EXTERNAL_REFA)
        {
            if (pinPeripheral(REFA_PIN, PIO_ANALOG_REF) != RET_STATUS_OK)
            {
                return;
            }
        }
#endif

#if defined(REFB_PIN)
        if (mode == AR_EXTERNAL_REFB)
        {
            if (pinPeripheral(REFB_PIN, PIO_ANALOG_REF) != RET_STATUS_OK)
            {
                return;
            }
        }
#endif

        if (mode == 0)
        { // Set to 1.0V for the SAML, 1.024V for the SAMC
            SUPC->VREF.reg &= ~SUPC_VREF_SEL_Msk;
        }
        else if (mode >= AR_INTREF_1V0)
        { // Values starting at AR_INTREF_1V0 are used for the Supply Controller reference (AR_INTREF)
            SUPC->VREF.reg &= ~SUPC_VREF_SEL_Msk;
            SUPC->VREF.reg |= SUPC_VREF_SEL(mode - AR_INTREF_1V0); // See eAnalogReference typedef in wiring_analog.h. AR_INTREF_1V0 = 6.
            mode = 0;
        }
        ADC->REFCTRL.bit.REFSEL = mode;
        syncADC();
        // Start conversion, since The first conversion after the reference is changed must not be used.
        uint32_t valueRead __attribute__((unused));
        ADC->CTRLA.bit.ENABLE = 0x01; // Enable ADC
        syncADC();
        ADC->SWTRIG.bit.START = 1;
        syncADC();
        while (ADC->INTFLAG.bit.RESRDY == 0)
        { // Waiting for conversion to complete
        }
        valueRead = ADC->RESULT.reg;  // Dummy read (will also clear the Data Ready flag)
        ADC->CTRLA.bit.ENABLE = 0x00; // Disable ADC
        syncADC();
        REFinitialized = 1;
    }

    uint32_t analogRead(uint32_t pin)
    {
        uint32_t valueRead = 0;
        if (!ADCinitialized)
        {
            initADC();
        }

#if defined(REMAP_ANALOG_PIN_ID)
        REMAP_ANALOG_PIN_ID(pin);
#endif

        // pinPeripheral now handles disabling the DAC (if active)
        if (pinPeripheral(pin, PIO_ANALOG_ADC) == RET_STATUS_OK)
        {
            ADC->INPUTCTRL.bit.MUXPOS = GetADC(pin); // Selection for the positive ADC input
            syncADC();
            ADC->CTRLA.bit.ENABLE = 0x01; // Enable ADC
            syncADC();
            // Start conversion
            ADC->SWTRIG.bit.START = 1;
            syncADC();
            // Store the value
            while (ADC->INTFLAG.bit.RESRDY == 0)
            { // Waiting for conversion to complete
            }
            valueRead = ADC->RESULT.reg;
            ADC->CTRLA.bit.ENABLE = 0x00; // Disable ADC
            syncADC();
        }
        return mapResolution(valueRead, _ADCResolution, _readResolution);
    }

    void analogWrite(uint32_t pin, uint32_t value)
    {
        if ((GetPort(pin) == 0) && (GetPin(pin) == 2 || GetPin(pin) == 5))
        {
            if (pinPeripheral(pin, PIO_ANALOG_DAC) != RET_STATUS_OK)
            {
                return;
            }
            if (!DACinitialized)
            {
                initDAC();
            }
            syncDAC();
            uint8_t DACNumber = 0x00;
            value = mapResolution(value, _writeResolution, 12);
            if ((GetPort(pin) == 0) && (GetPin(pin) == 5))
            {
                DACNumber = 0x01;
            }
            if (!dacEnabled[DACNumber])
            {
                dacEnabled[DACNumber] = true;
                DAC->CTRLA.bit.ENABLE = 0x00;             // Disable DAC controller (so that DACCTRL can be modified)
                delayMicroseconds(40);                    // Must delay for at least 30us when turning off while refresh is on due to L21 DAC errata
                DAC->DATA[DACNumber].reg = value & 0xFFF; // DACx on 12 bits.
                syncDAC();
                DAC->DACCTRL[DACNumber].bit.ENABLE = 0x01; // The DACx output is turned on.
                DAC->CTRLA.bit.ENABLE = 0x01;              // Enable DAC controller
                syncDAC();
                while ((DAC->STATUS.reg & (1 << DACNumber)) == 0)
                { // Must wait for DACx to start
                }
            }
            DAC->DATA[DACNumber].reg = value & 0xFFF; // DACx on 12 bits.
            syncDAC();
            return;
        }
        else if (g_APinDescription[pin].ulTCChannel != TIMER_NONE)
        {
            Tc *TCx = 0;
            Tcc *TCCx = 0;
            uint8_t timerIndex;
            uint8_t timerType = GetTCType(g_APinDescription[pin].ulTCChannel);
            uint8_t timerNumber = GetTCNumber(g_APinDescription[pin].ulTCChannel);
            uint8_t timerChannel = GetTCChannelNumber(g_APinDescription[pin].ulTCChannel);
            static bool timerEnabled[TCC_INST_NUM + TC_INST_NUM];
            if (pinPeripheral(pin, PIO_TIMER_PWM) != RET_STATUS_OK)
            {
                return;
            }
            if (timerType == 1)
            {
                TCx = (Tc *)GetTC(g_APinDescription[pin].ulTCChannel);
                timerIndex = (timerNumber + TCC_INST_NUM);
            }
            else
            {
                TCCx = (Tcc *)GetTC(g_APinDescription[pin].ulTCChannel);
                timerIndex = timerNumber;
            }

#if defined(TIMER_RESOLUTION_IS_16BIT)
            value = mapResolution(value, _writeResolution, 16);
#else
            value = mapResolution(value, _writeResolution, 8);
#endif

            if (!timerEnabled[timerIndex])
            {
                timerEnabled[timerIndex] = true;

                GCLK->PCHCTRL[timerClockIDs[timerIndex]].reg = (GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK5);
                while ((GCLK->PCHCTRL[timerClockIDs[timerIndex]].reg & GCLK_PCHCTRL_CHEN) == 0)
                { // wait for sync
                }

                // Set PORT. Note that COUNT16 usually maps to the same location as COUNT8, so COUNT16 is used in most cases with both 8-bit and 16-bit.
                if (TCx)
                {
                    // -- Configure TC
                    // Disable TCx
                    TCx->COUNT16.CTRLA.bit.ENABLE = 0;
                    syncTC_16(TCx);
#if defined(TIMER_RESOLUTION_IS_16BIT)
                    // Set Timer counter Mode to 16 bits, normal PWM
                    TCx->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
#else
                // Set Timer counter Mode to 8 bits, normal PWM
                TCx->COUNT8.CTRLA.reg |= TC_CTRLA_MODE_COUNT8;
                syncTC_16(TCx);
                // Set PER to maximum counter value
                TCx->COUNT8.PER.reg = 0xFF;
#endif
                    syncTC_16(TCx);
                    // Set TCx as normal PWM
                    TCx->COUNT16.WAVE.reg = TC_WAVE_WAVEGEN_NPWM;
                    syncTC_16(TCx);
                    // Set the initial value
#if defined(TIMER_RESOLUTION_IS_16BIT)
                    TCx->COUNT16.CC[timerChannel].reg = (uint16_t)value;
#else
                TCx->COUNT8.CC[timerChannel].reg = (uint8_t)value;
#endif
                    syncTC_16(TCx);
                    // Enable TCx
                    TCx->COUNT16.CTRLA.bit.ENABLE = 1;
                    syncTC_16(TCx);
                }
                else
                {
                    // -- Configure TCC
                    // Disable TCCx
                    TCCx->CTRLA.bit.ENABLE = 0;
                    syncTCC(TCCx);
                    // Set TCCx as normal PWM
                    TCCx->WAVE.reg |= TCC_WAVE_WAVEGEN_NPWM;
                    syncTCC(TCCx);
                    // Set the initial value
                    TCCx->CC[timerChannel].reg = (uint32_t)value;
                    syncTCC(TCCx);
                    // Set PER to maximum counter value
#if defined(TIMER_RESOLUTION_IS_16BIT)
                    TCCx->PER.reg = 0xFFFF;
#else
                    TCCx->PER.reg = 0xFF;
#endif
                    syncTCC(TCCx);
                    // Enable TCCx
                    TCCx->CTRLA.bit.ENABLE = 1;
                    syncTCC(TCCx);
                }
            }
            else
            {
                if (TCx)
                {
#if defined(TIMER_RESOLUTION_IS_16BIT)
                    TCx->COUNT16.CCBUF[timerChannel].reg = (uint16_t)value;
#else
                    TCx->COUNT8.CCBUF[timerChannel].reg = (uint8_t)value;
#endif
                    syncTC_16(TCx);
                }
                else
                {
                    TCCx->CCBUF[timerChannel].reg = (uint32_t)value;
                    syncTCC(TCCx);
                }
            }
        }

        // -- Defaults to digital write
        else if (pinPeripheral(pin, PIO_OUTPUT) == RET_STATUS_OK)
        {
            value = mapResolution(value, _writeResolution, 8);
            if (value < 128)
            {
                digitalWrite(pin, LOW);
            }
            else
            {
                digitalWrite(pin, HIGH);
            }
        }
    }

#ifdef __cplusplus
}
#endif