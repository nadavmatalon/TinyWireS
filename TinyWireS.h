/*==============================================================================================================*

    @file     TinyWireS.h
 
 *==============================================================================================================*/

#if 1
__asm volatile ("nop");
#endif

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

