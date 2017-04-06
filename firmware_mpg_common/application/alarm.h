/**********************************************************************************************************************
File: alarm.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this alarm as a template:
1. Follow the instructions at the top of alarm.c
2. Use ctrl-h to find and replace all instances of "alarm" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "Alarm" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "ALARM" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for alarm.c

**********************************************************************************************************************/

#ifndef __ALARM_H
#define __ALARM_H

//#define DEBUGALARM
/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
#define         ALARM_FREQ_1            10000
#define         ALARM_FREQ_2            5000
#define         CYCLE_PERIOD            100
/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

void activateAlarm(void);
void silenceAlarm(void);
/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void AlarmInitialize(void);
void AlarmRunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void AlarmSM_Idle(void);    

static void AlarmSM_Error(void);         
static void AlarmSM_FailedInit(void);        


#endif /* __ALARM_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/