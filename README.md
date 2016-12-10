# TinyWireS

[![Platform Badge](https://img.shields.io/badge/platform-tinyAVR-orange.svg)](http://www.atmel.com/products/microcontrollers/avr/tinyavr.aspx)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![SemVer](https://img.shields.io/badge/SemVer-1.0.0-brightgreen.svg)](http://semver.org/)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## INTRODUCTION

This library enables members of the ATtiny family to become Slaves on an I2C Bus. The library is based on a hardware implementation of I2C which is faster than the software one, but requries using a specific set of pins for SDA (PA6 on ATtiny84A) & SCL (PA4 on ATtiny84A).

## REPOSITORY CONTENTS

- **TinyWireS.h** - Library Header file.
- **TinyWireS.cpp** - Library Compilation file.
- **/utility** 
    - **USI_TWI_Slave.h** - Header file with the TWI-USI functionality.
    - **USI_TWI_Slave.c** - C\# file with relevant TWI-USI functions.
- **/examples**  
    - **/TinyWireS_Example**
        - **/ATtiny84A_Slave**
            - **ATtiny84A_Slave.ino** - Slave-Side: A basic sketch implementing the ATtiny84A as an I2C Slave and Arduino Uno as the I2C Master  
        - **Arduino_Uno_Master** 
            - **Arduino_Uno_Master.ino** - Master-Side: A basic sketch implementing the ATtiny84A as an I2C Slave and Arduino Uno as the I2C Master  
- **/extras** 
    - **License.txt** - A copy of the end-user license agreement.  
- **keywords.txt** - Keywords for this library which will be highlighted in sketches within the Arduino IDE. 
- **library.properties** - General library properties for the Arduino's IDE (>1.5) Library Package Manager.
- **README.md** - The readme file for this library.
- **library.json** - JSON file for the Arduino's IDE (>1.5) Package Manager.


## GENERAL NOTES

This library follows the convetions of the Arduino's built-in [Wire](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire) library, with only 'TinyWireS' replacing the 'Wire' keyword. Hence, for example, use TinyWireS.begin(SLAVE_ADDR) to join the I2C Bus as Slave, or TinyWireS.write(dataByte) to send a data byte to the Master.


## BUG REPORTS

Please report any issues/bugs/suggestions at the [Issues](https://github.com/nadavmatalon/TinyWireS/issues) section of this Github repository.


## TODO


## VERSION HISTORY

__Ver. 1.0.0__ - First release (4.12.16)  


## LICENSE

[The MIT License (MIT)](https://opensource.org/licenses/MIT)
Copyright (c) 2016 Nadav Matalon

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

