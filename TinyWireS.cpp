/*==============================================================================================================*
 
    @file     TinyWireS.cpp

 *==============================================================================================================*/

#if 1
__asm volatile ("nop");
#endif

extern "C" {
    #include <inttypes.h>
    #include "USI_TWI_Slave.h"
    #include <avr/interrupt.h>
}

#include "TinyWireS.h"
#include "Arduino.h"

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
