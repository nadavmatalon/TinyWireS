/*==============================================================================================================*
 
    @file     TinyWireS.cpp
    @license  MIT (c) 2016 Nadav Matalon

    ATtiny I2C Slave Library

    Ver. 1.0.0 - First release (1.12.16)
 
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

extern "C" {
    #include <inttypes.h>
    #include <avr/interrupt.h>
    #include "utility/USI_TWI_Slave.h"
}

#include "Arduino.h"
#include "TinyWireS.h"

USI_TWI_S::USI_TWI_S() {}

void USI_TWI_S::begin(uint8_t slaveAddr) {                  // initialize I2C lib
    usiTwiSlaveInit(slaveAddr);
}

void USI_TWI_S::write(uint8_t data) {                       // send it back to master
    usiTwiTransmitByte(data);
}

uint8_t USI_TWI_S::available() {                            // the bytes available that haven't been read yet
    return usiTwiAmountDataInReceiveBuffer();
}
 
uint8_t USI_TWI_S::read() {                                 // returns the bytes received one at a time
    return usiTwiReceiveByte();
}

void USI_TWI_S::onReceive(void (*function)(uint8_t)) {      // sets function called on slave write
    usi_onReceiverPtr = function;
}

void USI_TWI_S::onRequest(void (*function)(void)) {         // sets function called on slave read
    usi_onRequestPtr = function;
}

void TinyWireS_stop_check() {
    if (!usi_onReceiverPtr) return;                         // no onReceive callback, nothing to do...
    if (!(USISR & (1 << USIPF))) return;                    // Stop not detected
    uint8_t amount = usiTwiAmountDataInReceiveBuffer();
    if (amount == 0) return;                                // no data in buffer
    usi_onReceiverPtr(amount);
}

void tws_delay(unsigned long ms) {
    uint16_t start = (uint16_t)micros();
    while (ms > 0) {
        TinyWireS_stop_check();
        if (((uint16_t)micros() - start) >= 1000) {
            ms--;
            start += 1000;
        }
    }
}

USI_TWI_S TinyWireS = USI_TWI_S();
