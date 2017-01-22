#include "screen.h"
#include "configuration.h"
#include "watcher.h"

static fnCode_type currentScreenState;

void Screen_Initialize (void){
   currentScreenState = screenIdle;
}

void Screen_RunActiveState (void){
  currentScreenState();
}

void screenIdle(void){
  if (*(Watcher_GetAlertStatus()) != 0)
    currentScreenState = screenAlert;
}

void screenAlert(void){
}