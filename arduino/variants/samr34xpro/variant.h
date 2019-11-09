/*
  SAMR3 - variant
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

#ifndef __VARIANT_H__
#define __VARIANT_H__

#ifdef __cplusplus
extern "C"
{
#endif
// C ware

#define LED 4

int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);

#ifdef __cplusplus
}
//CPP ware

void initVariant();

//#include <HardwareSerial.h>
//extern HardwareSerial Serial;
//extern HardwareSerial Serial1;
//extern HardwareSerial Serial2;

#endif //__cplusplus

#endif /* __VARIANT_H__ */