/*==============================================================================================================*
 
    @file     TinyWireS.h
    @license  MIT (c) 2016 Nadav Matalon

    ATtiny I2C Slave Library

    Ver. 1.0.0 - First release (1.12.16)
 
 *===============================================================================================================*
    INTRODUCTION
 *===============================================================================================================*
 
    This library enables members of the ATtiny family to become Slaves on an I2C Bus. The library is based on
    a hardware implementation of I2C which is faster than the software one, but requries using a specific set 
    of pins for SDA (PA6 on ATtiny84A) & SCL (PA4 on ATtiny84A).
 
 *===============================================================================================================*
    LICENSE
 *===============================================================================================================*
 
    The MIT License (MIT)
    Copyright (c) 2016 Nadav Matalon

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
    documentation files (the "Software"), to deal in the Software without restriction, including without
    limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
    the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial
    portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
    LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 *==============================================================================================================*/

#ifndef TinyWireS_h
#define TinyWireS_h

#if !defined(ARDUINO_ARCH_AVR)
#error “The TinyWireS library only supports AVR processors.”
#endif

#include <inttypes.h>

class USI_TWI_S {
    public:
        USI_TWI_S();
        void    begin(uint8_t I2C_SLAVE_ADDR);
        void    write(uint8_t data);
        uint8_t available();
        uint8_t read();
        void    onReceive(void (*)(uint8_t));
        void    onRequest(void (*)(void));
};

void TinyWireS_stop_check();

void tws_delay(unsigned long);

extern USI_TWI_S TinyWireS;

#endif

