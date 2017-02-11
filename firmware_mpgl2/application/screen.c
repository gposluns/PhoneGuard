#include "screen.h"
#include "configuration.h"
#include "watcher.h"

static fnCode_type currentScreenState;
char** missingNames;
u8 listSize;

void Screen_Initialize (void){
   currentScreenState = screenIdle;
   listSize = 1;
   missingNames = (char*)malloc(sizeof(char));
}

void Screen_RunActiveState (void){
  currentScreenState();
}

void screenIdle(void){
}

void screenAlert(void){
}

void ScreenUpdateAlert (u8** info){
  
}