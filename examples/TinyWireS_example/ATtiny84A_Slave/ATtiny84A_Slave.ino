/*
    TinyWireS Library: ATtiny84A-Side (Slave)
    -----------------------------------------

    INTRODUCTION
    ------------
    The following sketch provides a simple example of setting up an ATtiny84A as an I2C Slave device
    on the I2C bus and an Arduino Uno as the I2C Master.

    At runtime, the Arduino Master will request 4 bytes from the ATtiny84A Slave and then print them
    out using the Serial Monitor.

    HW SETUP
    --------
    - Connect Arduino Pin A4 (HW SDA) to ATtiny84A Pin PA6 (SW SDA) with a 2K2 pull-up resistor to 5V
    - Connect Arduino Pin A5 (HW SCL) to ATtiny84A Pin PA4 (SW SCL) with a 2K2 pull-up resistor to 5V
    - Follow the basic hookup for Arduino & ATtiny84A (Common ground for both ICs, 100nF decoupling capacitors,
      10K pullup resistors for RESET pins, and if needed, external crystals)

    RUNNING THE SKETCH
    ------------------
    Upload this sketch to the ATtiny84A and the other example sketch (Arduino_Uno_Master.ino) to the
    Arduino. Then open the Serial Monitor (make sure the Baud Rate is set to 9600).

    BUG REPORTS
    -----------
    Please report any bugs/issues/suggestions at the GITHUB Repository of this library at:
    https://github.com/nadavmatalon/TinyWireS

    LICENSE
    -------
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
*/

#define __AVR_ATtiny84__

#include <Arduino.h>

#if defined(__AVR_ATtiny841__)
#define F_CPU 16000000                          // clock speed: 16MHz (external crystal)
#include "WireS.h"                              // I2C library for ATtiny841 (and other modern ATtinys)
#else
#define F_CPU 20000000                          // clock speed: 20MHz (external crystal)
#include "TinyWireS.h"                          // I2C library for ATtiny84A (and other older ATtinys)
#endif

const byte SLAVE_ADDR = 100;

const byte NUM_BYTES = 4;

volatile byte data[NUM_BYTES] = { 0, 1, 2, 3 };

void setup() {
    TinyWireS.begin(SLAVE_ADDR);
    TinyWireS.onRequest(requestISR);
}

void loop() {}

void requestISR() {
    for (byte i=0; i<NUM_BYTES; i++) {
        TinyWireS.write(data[i]);
        data[i] += 1;
    }
}