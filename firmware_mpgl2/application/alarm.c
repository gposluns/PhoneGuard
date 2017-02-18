/**********************************************************************************************************************
File: alarm.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this alarm as a template:
 1. Copy both alarm.c and alarm.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "alarm" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "Alarm" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "ALARM" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a alarm.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void AlarmInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void AlarmRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32AlarmFlags;                       /* Global state flags */
u8 toggle = 0;
u32 counter = 0;
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
static fnCode_type Alarm_StateMachine;            /* The state machine function pointer */
//static u32 Alarm_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: AlarmInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void AlarmInitialize(void)
{
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    Alarm_StateMachine = AlarmSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    Alarm_StateMachine = AlarmSM_FailedInit;
  }

} /* end AlarmInitialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function AlarmRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void AlarmRunActiveState(void)
{
  Alarm_StateMachine();

} /* end AlarmRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void AlarmSM_Idle(void)
{
    
} /* end AlarmSM_Idle() */
     

static void AlarmSM_Active(void){
  //TODO:Annoying alarm sounds/lights
  if (!toggle){
    LedPWM(GREEN0, LED_PWM_50);
    LedOff(GREEN1);
    LedPWM(GREEN2, LED_PWM_50);
    LedOff(GREEN3);
    PWMAudioSetFrequency(BUZZER1, 15000);
  }
  else{
    LedOff (GREEN0);
    LedPWM (GREEN1, LED_PWM_50);
    LedOff (GREEN2);
    LedPWM (GREEN3, LED_PWM_50);
    PWMAudioSetFrequency(BUZZER1, 5000);
  }
  if (counter > CYCLE_PERIOD){
    toggle = toggle==0?1:0;
    counter = 0;
  }
}

void activateAlarm(void){
  Alarm_StateMachine = AlarmSM_Active;
  LedOn (RED0);
  LedOn (RED1);
  LedOn (RED2);
  LedOn (RED3);
  LedPWM (GREEN1, LED_PWM_50);
  LedPWM (GREEN3, LED_PWM_50);
  PWMAudioOn(BUZZER1);
}

void silenceAlarm(void){
  Alarm_StateMachine = AlarmSM_Idle;
  toggle = 0;
  counter = 0;
  LedOff (RED0);
  LedOff (RED1);
  LedOff (RED2);
  LedOff (RED3);
  LedOff (GREEN0);
  LedOff (GREEN1);
  LedOff (GREEN2);
  LedOff (GREEN3);
  PWMAudioOff(BUZZER1);
  //TODO: Turn off lights/sound
}
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void AlarmSM_Error(void)          
{
  
} /* end AlarmSM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void AlarmSM_FailedInit(void)          
{
    
} /* end AlarmSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
