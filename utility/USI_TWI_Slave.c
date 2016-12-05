/*==============================================================================================================*
 
    @file     USI_TWI_Slave.c
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include "USI_TWI_Slave.h"

#if defined(__AVR_ATtiny2313__)
#  define DDR_USI             DDRB
#  define PORT_USI            PORTB
#  define PIN_USI             PINB
#  define PORT_USI_SDA        PB5
#  define PORT_USI_SCL        PB7
#  define PIN_USI_SDA         PINB5
#  define PIN_USI_SCL         PINB7
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVERFLOW_vect
#endif

#if defined(__AVR_ATtiny84__) |                 \
    defined(__AVR_ATtiny44__)
#  define DDR_USI             DDRA
#  define PORT_USI            PORTA
#  define PIN_USI             PINA
#  define PORT_USI_SDA        PORTA6
#  define PORT_USI_SCL        PORTA4
#  define PIN_USI_SDA         PINA6
#  define PIN_USI_SCL         PINA4
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

#if defined(__AVR_ATtiny25__) |                 \
    defined(__AVR_ATtiny45__) |                 \
    defined(__AVR_ATtiny85__)
#  define DDR_USI             DDRB
#  define PORT_USI            PORTB
#  define PIN_USI             PINB
#  define PORT_USI_SDA        PB0
#  define PORT_USI_SCL        PB2
#  define PIN_USI_SDA         PINB0
#  define PIN_USI_SCL         PINB2
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

#if defined(__AVR_ATtiny261__) |                \
    defined(__AVR_ATtiny461__) |                \
    defined(__AVR_ATtiny861__)
#  define DDR_USI             DDRB
#  define PORT_USI            PORTB
#  define PIN_USI             PINB
#  define PORT_USI_SDA        PB0
#  define PORT_USI_SCL        PB2
#  define PIN_USI_SDA         PINB0
#  define PIN_USI_SCL         PINB2
#  define USI_START_COND_INT  USISIF
#  define USI_START_VECTOR    USI_START_vect
#  define USI_OVERFLOW_VECTOR USI_OVF_vect
#endif

#define SET_USI_TO_SEND_ACK() {                                 \
  USIDR    =  0;                                                \
  DDR_USI |= (1 << PORT_USI_SDA);                               \
  USISR    = (0 << USI_START_COND_INT)  | (1 << USIOIF) |       \
             (1 << USIPF) | (1 << USIDC)| (0x0E << USICNT0);    \
}

#define SET_USI_TO_READ_ACK() {                                     \
    DDR_USI &= ~(1 << PORT_USI_SDA);                                \
    USIDR    =   0;                                                 \
    USISR    =  (0 << USI_START_COND_INT)   | (1 << USIOIF) |       \
                (1 << USIPF) | (1 << USIDC) | (0x0E << USICNT0);    \
}

#define SET_USI_TO_TWI_START_CONDITION_MODE() {                                   \
    USICR = (1 << USISIE) | (0 << USIOIE) | (1 << USIWM1) | (0 << USIWM0) |       \
            (1 << USICS1) | (0 << USICS0) | (0 << USICLK) | (0 << USITC);         \
    USISR = (0 << USI_START_COND_INT) | (1 << USIOIF) |                           \
            (1 << USIPF)  | (1 << USIDC)  | (0x0 << USICNT0);                     \
}

#define SET_USI_TO_SEND_DATA() {                                              \
    DDR_USI |= (1 << PORT_USI_SDA);                                           \
    USISR    = (0 << USI_START_COND_INT)    | (1 << USIOIF) |                 \
               (1 << USIPF) | ( 1 << USIDC) | ( 0x0 << USICNT0 );             \
}

#define SET_USI_TO_READ_DATA() { \
    DDR_USI &= ~(1 << PORT_USI_SDA);                                          \
    USISR    =  (0 << USI_START_COND_INT)   | (1 << USIOIF) |                 \
                (1 << USIPF) | (1 << USIDC) | (0x0 << USICNT0);               \
}

#define USI_RECEIVE_CALLBACK() {                                              \
    if (usi_onReceiverPtr) {                                                  \
        if (usiTwiAmountDataInReceiveBuffer()) {                              \
            usi_onReceiverPtr(usiTwiAmountDataInReceiveBuffer());             \
        }                                                                     \
    }                                                                         \
}

#define ONSTOP_USI_RECEIVE_CALLBACK() {                                       \
    if (USISR & (1 << USIPF)) {                                               \
        USI_RECEIVE_CALLBACK();                                               \
    }                                                                         \
}

#define USI_REQUEST_CALLBACK() {                                              \
    USI_RECEIVE_CALLBACK();                                                   \
    if(usi_onRequestPtr) usi_onRequestPtr();                                  \
}

typedef enum {
    USI_SLAVE_CHECK_ADDRESS                = 0x00,
    USI_SLAVE_SEND_DATA                    = 0x01,
    USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA = 0x02,
    USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA   = 0x03,
    USI_SLAVE_REQUEST_DATA                 = 0x04,
    USI_SLAVE_GET_DATA_AND_SEND_ACK        = 0x05
} overflowState_t;

static uint8_t slaveAddress;
static volatile overflowState_t overflowState;

static uint8_t rxBuf[ TWI_RX_BUFFER_SIZE ];
static volatile uint8_t rxHead;
static volatile uint8_t rxTail;
static volatile uint8_t rxCount;

static uint8_t txBuf[ TWI_TX_BUFFER_SIZE ];
static volatile uint8_t txHead;
static volatile uint8_t txTail;
static volatile uint8_t txCount;

static void flushTwiBuffers(void) {
    rxTail  = 0;
    rxHead  = 0;
    rxCount = 0;
    txTail  = 0;
    txHead  = 0;
    txCount = 0;
}

void usiTwiSlaveInit(uint8_t ownAddress) {
    flushTwiBuffers();
    slaveAddress = ownAddress;
    DDR_USI  |=  (1 << PORT_USI_SCL) | (1 << PORT_USI_SDA);
    PORT_USI |=  (1 << PORT_USI_SCL);
    PORT_USI |=  (1 << PORT_USI_SDA);
    DDR_USI  &= ~(1 << PORT_USI_SDA);
    USICR     =  (1 << USISIE) | (0 << USIOIE) | (1 << USIWM1) | (0 << USIWM0) |
                 (1 << USICS1) | (0 << USICS0) | (0 << USICLK) | (0 << USITC);
    USISR     =  (1 << USI_START_COND_INT) | (1 << USIOIF) | (1 << USIPF) | (1 << USIDC);
}


bool usiTwiDataInTransmitBuffer(void) {
    return txCount;
}

void usiTwiTransmitByte(uint8_t data) {
    uint8_t tmphead;
    while (txCount == TWI_TX_BUFFER_SIZE);
    txBuf[txHead] = data;
    txHead = (txHead + 1) & TWI_TX_BUFFER_MASK;
    txCount++;
}

uint8_t usiTwiReceiveByte(void) {
    uint8_t rtn_byte;
    while (!rxCount);
    rtn_byte = rxBuf [rxTail];
    rxTail = (rxTail + 1) & TWI_RX_BUFFER_MASK;
    rxCount--;
    return rtn_byte;
}

uint8_t usiTwiAmountDataInReceiveBuffer(void) {
    return rxCount;
}
 
ISR(USI_START_VECTOR) {
    overflowState = USI_SLAVE_CHECK_ADDRESS;
    DDR_USI &= ~(1 << PORT_USI_SDA);
    while ((PIN_USI & (1 << PIN_USI_SCL)) && !((PIN_USI & (1 << PIN_USI_SDA))));
    if (!(PIN_USI & (1 << PIN_USI_SDA))) {
        USICR = (1 << USISIE) | (1 << USIOIE) | (1 << USIWM1) | (1 << USIWM0) |
                (1 << USICS1) | (0 << USICS0) | (0 << USICLK) | (0 << USITC);
    } else {
        USICR = (1 << USISIE) | (0 << USIOIE) | (1 << USIWM1 ) | (0 << USIWM0) |
                (1 << USICS1) | (0 << USICS0) | (0 << USICLK ) | (0 << USITC);
    }
    USISR = (1 << USI_START_COND_INT)   | (1 << USIOIF) |
            (1 << USIPF) | (1 << USIDC) | (0x0 << USICNT0);
}

ISR(USI_OVERFLOW_VECTOR) {
    switch (overflowState) {
        case (USI_SLAVE_CHECK_ADDRESS):
            if ((USIDR == 0) || ((USIDR >> 1) == slaveAddress)) {
                if (USIDR & 0x01) {
                    USI_REQUEST_CALLBACK();
                    overflowState = USI_SLAVE_SEND_DATA;
                } else overflowState = USI_SLAVE_REQUEST_DATA;
                SET_USI_TO_SEND_ACK();
            } else SET_USI_TO_TWI_START_CONDITION_MODE();
            break;
        case (USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA):
            if (USIDR) {
                SET_USI_TO_TWI_START_CONDITION_MODE();
                return;
            }
        case (USI_SLAVE_SEND_DATA):
            if (txCount) {
                USIDR = txBuf[txTail];
                txTail = (txTail + 1) & TWI_TX_BUFFER_MASK;
                txCount--;
            } else {
                SET_USI_TO_READ_ACK();
                SET_USI_TO_TWI_START_CONDITION_MODE();
                return;
            }
            overflowState = USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA;
            SET_USI_TO_SEND_DATA();
            break;
        case (USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA):
            overflowState = USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA;
            SET_USI_TO_READ_ACK();
            break;
        case (USI_SLAVE_REQUEST_DATA):
            overflowState = USI_SLAVE_GET_DATA_AND_SEND_ACK;
            SET_USI_TO_READ_DATA();
            break;
        case (USI_SLAVE_GET_DATA_AND_SEND_ACK):
            if (rxCount < TWI_RX_BUFFER_SIZE) {
                    rxBuf[rxHead] = USIDR;
                    rxHead = (rxHead + 1) & TWI_RX_BUFFER_MASK;
                    rxCount++;
            }
            overflowState = USI_SLAVE_REQUEST_DATA;
            SET_USI_TO_SEND_ACK();
            break;
    }

}
