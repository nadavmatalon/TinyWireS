/*
    TinyWireS Library: Arduino-Uno-Side (Master)
    --------------------------------------------

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
    - Follow the basic hookup for Arduino & ATtiny84A (Common Ground for both ICs, 100nF decoupling capacitors,
      10K pullup resistors for RESET pins, and if needed, external crystals)

    RUNNING THE SKETCH
    ------------------
    Upload this sketch to the Arduino Uno and the other example sketch (ATtiny84A_Slave.ino) to the
    ATtiny84A. Then open the Serial Monitor (make sure the Baud Rate is set to 9600).

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

#include <Arduino.h>
#include <Wire.h>

const byte SLAVE_ADDR = 100;

const byte NUM_BYTES = 4;

byte data[NUM_BYTES] = { 0 };

byte bytesReceived = 0;

unsigned long timeNow = millis();

void setup() {
    Serial.begin(9600);
    Wire.begin();
    Serial.print(F("\n\nSerial is Open\n\n"));
}

void loop() {
    if (millis() - timeNow >= 750) {                                        // trigger every 750mS
        Wire.requestFrom(SLAVE_ADDR, NUM_BYTES);                            // request bytes from slave
        bytesReceived = Wire.available();                                   // count how many bytes received
        if (bytesReceived == NUM_BYTES) {                                   // if received correct number of bytes...
            for (byte i=0; i<NUM_BYTES; i++) data[i] = Wire.read();         // read and store each byte
            printData();                                                    // print the received data
        } else {                                                            // if received wrong number of bytes...
            Serial.print(F("\nRequested "));                                // print message with how many bytes received
            Serial.print(NUM_BYTES);
            Serial.print(F(" bytes, but got "));
            Serial.print(bytesReceived);
            Serial.print(F(" bytes\n"));
        }
        timeNow = millis();                                                 // mark preset time for next trigger
    }
}

void printData() {
    Serial.print(F("\n"));
    for (byte i=0; i<NUM_BYTES; i++) {
          Serial.print(F("Byte["));
          Serial.print(i);
          Serial.print(F("]: "));
          Serial.print(data[i]);
          Serial.print(F("\t"));
    }
    Serial.print(F("\n"));    
}
