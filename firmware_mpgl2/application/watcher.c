/**********************************************************************************************************************
File: watcher.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this watcher as a template:
 1. Copy both watcher.c and watcher.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "watcher" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "Watcher" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "WATCHER" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a watcher.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void WatcherInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void WatcherRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32WatcherFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type Watcher_StateMachine;            /* The state machine function pointer */
//static u32 Watcher_u32Timeout;                      /* Timeout counter used across states */

static u8 REGISTER_ACKNOWLEDGE[] = {253, 0, 0, 0, 0, 0, 0, 0, 0};
static u8 RELEASE_ACKNOWLEDGE[] = {252, 0, 0, 0, 0, 0, 0, 0, 0};
static u8 REGISTER_FAIL_NAME_CONFLICT[] = {251, 0, 0, 0, 0, 0, 0, 0, 0};
static u8 RELEASE_FAIL_NAME_NOT_FOUND[] = {250, 0, 0, 0, 0, 0, 0, 0, 0};
static u8 NORMAL[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

u8 deviceNames[8][8];
u8 numDevices;
u8 watcher_listSize = 8;
u32 lastResponseTimes[8];
u32 lastTime;
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
void watcherNoteResponse(u8* message){
  int i;
  for (i = 0; i < watcher_listSize; i++){
    if (!strcmp(message + 1, deviceNames[i])){
      lastResponseTimes[i] = 0;
      WatcherSM_Idle();  //I am incredibly lazy
      return;
    }
  }
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: WatcherInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void WatcherInitialize(void)
{
  lastTime = G_u32SystemTime1s; 
  numDevices = 0;
  watcher_listSize = DEFAULT_LIST_SIZE;;
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    Watcher_StateMachine = WatcherSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    Watcher_StateMachine = WatcherSM_FailedInit;
  }

} /* end WatcherInitialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function WatcherRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void WatcherRunActiveState(void)
{
  Watcher_StateMachine();

} /* end WatcherRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

void WatcherRegister(u8* message){
  int i, j;
  for (i = 0; i < 8; i++){
    if (!strcmp(message + 1, deviceNames[i])){
      AntQueueBroadcastMessage (REGISTER_FAIL_NAME_CONFLICT);
      return;
    }
  }
  numDevices++;
  for (i = 0; i < 8; i++){
    if (deviceNames[i][0] == '\0'){
      for (j = 0; j<MAX_NAME_LENGTH; j++){
        deviceNames[i][j] = message[j + 1];
        REGISTER_ACKNOWLEDGE[j + 1] = message[j + 1];
      }
      deviceNames[i][MAX_NAME_LENGTH] = '\0';
    }
  }
  AntQueueBroadcastMessage(REGISTER_ACKNOWLEDGE);
}

void WatcherRelease(u8* message){
  int i;
  for (i = 0; i < MAX_NAME_LENGTH; i++){
    message[i] = message[i + 1];
  }
  message[MAX_NAME_LENGTH] = '\0';
  for (i = 0; i < watcher_listSize;i++){
    if (!strcmp(message, deviceNames[i])){
      deviceNames[i][0] = '\0';
      numDevices--;
      RELEASE_ACKNOWLEDGE [1] = message[1];
      RELEASE_ACKNOWLEDGE [2] = message[2];
      RELEASE_ACKNOWLEDGE [3] = message[3];
      RELEASE_ACKNOWLEDGE [4] = message[4];
      RELEASE_ACKNOWLEDGE [5] = message[5];
      RELEASE_ACKNOWLEDGE [6] = message[6];
      RELEASE_ACKNOWLEDGE [7] = message[7];
      AntQueueBroadcastMessage(RELEASE_ACKNOWLEDGE);
      break;
    }
    if (i == watcher_listSize - 1){
      RELEASE_FAIL_NAME_NOT_FOUND [1] = message[1];
      RELEASE_FAIL_NAME_NOT_FOUND [2] = message[2];
      RELEASE_FAIL_NAME_NOT_FOUND [3] = message[3];
      RELEASE_FAIL_NAME_NOT_FOUND [4] = message[4];
      RELEASE_FAIL_NAME_NOT_FOUND [5] = message[5];
      RELEASE_FAIL_NAME_NOT_FOUND [6] = message[6];
      RELEASE_FAIL_NAME_NOT_FOUND [7] = message[7];
      AntQueueBroadcastMessage(RELEASE_FAIL_NAME_NOT_FOUND);
    }
  }
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

void WatcherAlert(){
  u8 i, n = 0;
  u8* missing[8];
  for (i = 0; i < watcher_listSize && n < numDevices; i++){
    if (lastResponseTimes[i] > MAX_WAIT){
      missing[n++] = deviceNames[i];
    }
  }
  if (n == 0){
    Watcher_StateMachine = WatcherSM_Idle;
#ifndef DEBUGALARM
    silenceAlarm();
#endif
  }
  else {
    activateAlarm();
  }
  ScreenUpdateAlert (missing, n);
  WatcherSM_Idle();
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void WatcherSM_Idle(void)
{
    int i;
    for (i = 0; i < numDevices; i++){
      if (deviceNames[i] == NULL)
        continue;
      lastResponseTimes[i]+= G_u32SystemTime1s - lastTime;
      if (lastResponseTimes[i] > MAX_WAIT){
        Watcher_StateMachine = WatcherAlert;
      }
    }
    lastTime = G_u32SystemTime1s;
    
} /* end WatcherSM_Idle() */
     
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void WatcherSM_Error(void)          
{
  
} /* end WatcherSM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void WatcherSM_FailedInit(void)          
{
    
} /* end WatcherSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
