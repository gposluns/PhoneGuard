#ifndef connect
#define connect
//unsigned char* recieved;

void ConnectInitialize(void);
void ConnectRunActiveState(void);

void connectIdle(void);
void connectProcessMessage(void);
void connectQueue(u8* message);

#define ANT_CHANNEL_CONNECT             (u8)0                  // Channel 0 - 7
#define ANT_SERIAL_LO_CONNECT           (u8)0x22                  // Low byte of two-byte Device #
#define ANT_SERIAL_HI_CONNECT           (u8)0x00                  // High byte of two-byte Device #
#define ANT_DEVICE_TYPE_CONNECT         (u8)1                  // 1 - 255
#define ANT_TRANSMISSION_TYPE_CONNECT   (u8)1                  // 1-127 (MSB is pairing bit)
#define ANT_CHANNEL_PERIOD_LO_CONNECT   (u8)0xff            // Low byte of two-byte channel period 0x0001 - 0x7fff
#define ANT_CHANNEL_PERIOD_HI_CONNECT   (u8)0x7f             // High byte of two-byte channel period 
#define ANT_FREQUENCY_CONNECT           (u8)50               // 2400MHz + this number 0 - 99
#define ANT_TX_POWER_CONNECT            RADIO_TX_POWER_0DBM   // RADIO_TX_POWER_0DBM, RADIO_TX_POWER_MINUS5DBM, RADIO_TX_POWER_MINUS10DBM, RADIO_TX_POWER_MINUS20DBM

#define REGISTER                        255
#define RELEASE                         254
#endif