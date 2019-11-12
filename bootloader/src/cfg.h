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

#ifndef _CFG_H_
#define _CFG_H_

#include <stdint.h>
#include <string.h>
#include <samr34.h>

#define BOOT_DELAY         0xFFFFF

#define APPLICATION        0x00002000  /* start address      */
#define MIN_ADDRESS        0x02000     /* protect bootloader */
#define MAX_ADDRESS        0x40000     /* from linker script */

#define CLOCK_DEFAULT      48000000ul

#define BOOT_UART          SERCOM0

#define ATTN        '#' 
#define CONF        'C'
#define ACK         'A'
#define NACK        'N'

#define PING        'P'
#define PONG        'p'

#define DA_ERASE    'E'
#define DA_WRITE    'W'
#define DA_READ     'R'
#define DA_RESET    'X'

typedef enum
{
    E_LIMIT = 1,
    E_CRC,
    E_ALIGN,
    E_ERASE,
    E_WRITE,
    E_VERIFY,
} ERRORS_E;

void UART_INIT(void);
int available(void);
uint8_t getChar(void);
void putChar(char);
void putString(char *str);
void boot_task(void);

#endif