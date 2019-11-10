/*
   
   Microchip Atmel SAMR3x Flash Utility ver 1.00 PlatformIO
  
     Copyright (C) 2019 Georgi Angelov. All rights reserved.
     Author: Georgi Angelov <the.wizarda@gmail.com> WizIO
         http://www.wizio.eu/
         https://github.com/Wiz-IO/platform-sam-lora
  
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
  
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. Neither the name WizIO nor the names of its contributors may be
      used to endorse or promote products derived from this software
      without specific prior written permission.
  
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
   COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
   OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
  
*/

#include <stdint.h>
#include <string.h>
#include <samr34.h>
#include "cfg.h"

void delayUs(uint32_t delay);

static void UART_RESET(Sercom *s)
{
    s->USART.CTRLA.bit.SWRST = 1;
    while (s->USART.CTRLA.bit.SWRST || s->USART.SYNCBUSY.bit.SWRST)
    {
    }
}

static void UART_WAIT_BUSY(Sercom *s)
{
    while (s->USART.SYNCBUSY.bit.ENABLE)
    {
    }
}

void UART_INIT(void) // 115200 8N1
{
    //Sercom * s = BOOT_UART;
    uint32_t temp;
    // EVEN PIN 4 TX
    temp = PORT->Group[0].PMUX[4 >> 1].reg & PORT_PMUX_PMUXO(0xF);
    PORT->Group[0].PMUX[4 >> 1].reg = temp | PORT_PMUX_PMUXE(3); // PIO_SERCOM_ALT
    PORT->Group[0].PINCFG[4].reg |= PORT_PINCFG_PMUXEN;
    // ODD PIN 5 RX
    temp = PORT->Group[0].PMUX[5 >> 1].reg & PORT_PMUX_PMUXE(0xF);
    PORT->Group[0].PMUX[5 >> 1].reg = temp | PORT_PMUX_PMUXO(3); // PIO_SERCOM_ALT
    PORT->Group[0].PINCFG[5].reg |= PORT_PINCFG_PMUXEN;
    // Init uatr clock
    GCLK->PCHCTRL[18].reg = (GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN_GCLK0);
    while (!(GCLK->PCHCTRL[18].reg & GCLK_PCHCTRL_CHEN))
    {
    }
    // Start the Software Reset
    UART_RESET(BOOT_UART);
    /* Write settings */
    BOOT_UART->USART.CTRLA.reg = 0x40102004;
    BOOT_UART->USART.CTRLB.reg = 0x00030000;
    BOOT_UART->USART.BAUD.reg = 26;
    UART_WAIT_BUSY(BOOT_UART);
    /* enable usart */
    BOOT_UART->USART.CTRLA.bit.ENABLE = 1;
    UART_WAIT_BUSY(BOOT_UART);

    delayUs(10000); // ????
}

int available(void)
{
    return BOOT_UART->USART.INTFLAG.bit.RXC;
}

uint8_t getChar(void)
{
    return BOOT_UART->USART.DATA.reg;
}

void getBuffer(uint8_t *buf, int len)
{
    while (len--)
    {
        while (0 == BOOT_UART->USART.INTFLAG.bit.RXC) // not available()
        {
        }
        *buf++ = BOOT_UART->USART.DATA.reg;
    }
}

void putChar(char c)
{
    while (BOOT_UART->USART.INTFLAG.bit.DRE == 0)
    {
    }
    BOOT_UART->USART.DATA.reg = c;
}

void putBuffer(uint8_t *buf, int size)
{
    while (size--)
        putChar(*buf++);
}

void putString(char *str)
{
    while (*str)
        putChar(*str++);
}

void flush(void)
{
    while (BOOT_UART->USART.INTFLAG.bit.RXC)
        getChar();
}

void send_error(char err)
{
    putChar(NACK);
    putChar(err);
    for (int i = 0; i < 10000; i++)
        flush(); // flush delay
}

void send_answer(char a)
{
    putChar(a);
    putChar(a);
}

uint16_t checksum(uint16_t seed, uint8_t *buff, int size)
{
    uint16_t crc = seed;
    while (size--)
        crc += *buff++;
    return crc;
}

int is_limit(uint32_t addr)
{
    if (addr < MIN_ADDRESS || addr > MAX_ADDRESS)
    {
        send_error(E_LIMIT);
        return -1;
    }
    return 0;
}

int is_wrong_crc(uint8_t *p, int size)
{
    if (checksum(0, p, size - 2) != *((uint16_t *)&p[size - 2]))
    {
        send_error(E_CRC);
        return -1;
    }
    return 0;
}

int is_not_align(uint32_t addr, int mask)
{
    if (addr & mask)
    {
        send_error(E_ALIGN);
        return -1;
    }
    return 0;
}

void cmd_erase(void)
{
    struct __attribute__((__packed__)) p_erase
    {
        uint32_t addr;
        uint16_t crc;
    } p = {0};
    getBuffer((uint8_t *)&p, sizeof(struct p_erase));
    if (is_limit(p.addr) ||
        is_wrong_crc((uint8_t *)&p, sizeof(struct p_erase)) ||
        is_not_align(p.addr, 255))
        return;
    // DO ERASE
    send_answer(CONF);
}

void cmd_write(void)
{
    struct __attribute__((__packed__)) p_write
    {
        uint32_t addr;
        uint8_t buff[64];
        uint16_t crc;
    } p = {0};
    getBuffer((uint8_t *)&p, sizeof(struct p_write));
    if (is_limit(p.addr) ||
        is_wrong_crc((uint8_t *)&p, sizeof(struct p_write)) ||
        is_not_align(p.addr, 63))
        return;
    // DO WRITE
    if (memcmp((uint8_t *)p.addr, p.buff, 64))
        send_error(E_VERIFY);
    else
        send_answer(CONF);
}

void cmd_read(void)
{
    struct __attribute__((__packed__)) p_read
    {
        uint32_t addr;
        uint16_t size;
        uint16_t crc;
    } p = {0};
    getBuffer((uint8_t *)&p, sizeof(struct p_read));
    if (is_limit(p.addr) ||
        is_wrong_crc((uint8_t *)&p, sizeof(struct p_read)))
        return;
    if (p.size > 1024) // max
        p.size = 1024;
    send_answer(CONF);
    char *d = (char *)p.addr;
    while (p.size--)
        putChar(*d++);
}

void cmd_reset(void)
{
    send_answer(CONF);
    delayUs(1000);
    NVIC_SystemReset();
    while (1)
    {
    }
}

void boot_task(void)
{
    putString("BOOT");
    flush();
    while (1)
    {
    NEW_LINE:
        while (1) // wait attention
        {
            if (BOOT_UART->USART.INTFLAG.bit.RXC)
                if (ATTN == BOOT_UART->USART.DATA.reg)
                    break;
        }
        while (1) // wait command
        {
            if (BOOT_UART->USART.INTFLAG.bit.RXC)
            {
                switch (BOOT_UART->USART.DATA.reg)
                {
                case DA_ERASE:
                    cmd_erase();
                    goto NEW_LINE;

                case DA_WRITE:
                    cmd_write();
                    goto NEW_LINE;

                case DA_READ:
                    cmd_read();
                    goto NEW_LINE;

                case DA_RESET:
                    cmd_reset();
                    break;

                case PING:
                    send_answer(PONG);
                    goto NEW_LINE;

                default:
                    send_error(0xF0);
                    goto NEW_LINE;
                } // switch

            } // available

        } // while command
    }
}
