#include "configuration.h"

static fnCode_type connectActiveState;

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern AntSetupDataType G_stAntSetupData;                         /* From ant.c */

extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentData[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */

u8 message[] = {0, 0, 0, 0, 0, 0, 0, 0}; 

void Connect_RunActiveState(void){
  connectActiveState();
}

void Connect_Initialize(){
  G_stAntSetupData.AntChannel          = ANT_CHANNEL_CONNECT;
  G_stAntSetupData.AntSerialLo         = ANT_SERIAL_LO_CONNECT;
  G_stAntSetupData.AntSerialHi         = ANT_SERIAL_HI_CONNECT;
  G_stAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_CONNECT;
  G_stAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_CONNECT;
  G_stAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_CONNECT;
  G_stAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_CONNECT;
  G_stAntSetupData.AntFrequency        = ANT_FREQUENCY_CONNECT;
  G_stAntSetupData.AntTxPower          = ANT_TX_POWER_CONNECT;
  
  AntChannelConfig(ANT_MASTER);
  AntOpenChannel();
  
  connectActiveState = connectIdle;
}

void connectIdle(){
  if (AntReadData()){
       connectProcessMessage();
   }
}

void connectProcessMessage(){
  if (G_eAntApiCurrentMessageClass == ANT_TICK){
    //0s are fine, its response i care about
  }
  else if(G_eAntApiCurrentMessageClass == ANT_DATA){
    switch (*G_au8AntApiCurrentData){
      case REGISTER: 
        WatcherRegister(G_au8AntApiCurrentData);
        break;
      case RELEASE:
        WatcherRelease(G_au8AntApiCurrentData);
        break;
      default:
        watcherNoteResponse(G_au8AntApiCurrentData);
    }
  }
}