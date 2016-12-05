
#ifndef USI_TWI_Slave_h
#define USI_TWI_Slave_h

#include <stdbool.h>

void    usiTwiSlaveInit( uint8_t );
void    usiTwiTransmitByte( uint8_t );
uint8_t usiTwiReceiveByte( void );
bool    usiTwiDataInReceiveBuffer( void );
void    (*_onTwiDataRequest)(void);
bool    usiTwiDataInTransmitBuffer(void);
uint8_t usiTwiAmountDataInReceiveBuffer(void);
void    (*usi_onRequestPtr)(void);
void    (*usi_onReceiverPtr)(uint8_t);

#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE (32)
#endif

#define TWI_RX_BUFFER_MASK (TWI_RX_BUFFER_SIZE - 1)

#if (TWI_RX_BUFFER_SIZE & TWI_RX_BUFFER_MASK)
#error TWI RX buffer size is not a power of 2
#endif

#ifndef TWI_TX_BUFFER_SIZE
#define TWI_TX_BUFFER_SIZE (32)
#endif

#define TWI_TX_BUFFER_MASK (TWI_TX_BUFFER_SIZE - 1)

#if (TWI_TX_BUFFER_SIZE & TWI_TX_BUFFER_MASK)
#error TWI TX buffer size is not a power of 2
#endif

#endif
