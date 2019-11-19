#include <interface.h>

uint32_t convert_byte_array_to_32_bit(uint8_t *data)
{
    union {
        uint32_t u32;
        uint8_t u8[4];
    } long_addr;
    uint8_t index;
    for (index = 0; index < 4; index++)
        long_addr.u8[index] = *data++;
    return long_addr.u32;
}

#if DEBUG_ENABLE > 0

#define DBG_UART SERCOM0

char DBG_BUFFER[DBG_BUF_LEN];

void putChar(char c)
{
    if (SERCOM0->USART.CTRLA.bit.ENABLE == 0)
        return;
    while (SERCOM0->USART.INTFLAG.bit.DRE == 0)
    {
    }
    DBG_UART->USART.DATA.reg = c;
}

void putString(char *str)
{
    while (*str)
        putChar(*str++);
}

#endif


#include <samr3.h>
uint32_t rnd(void)
{
    TRNG->CTRLA.reg |= TRNG_CTRLA_ENABLE;
    while (((TRNG->INTFLAG.reg) & (TRNG_INTFLAG_DATARDY)) != TRNG_INTFLAG_DATARDY)
    {
    }
    TRNG->CTRLA.reg &= ~(TRNG_CTRLA_ENABLE);
    return (TRNG->DATA.reg);
}