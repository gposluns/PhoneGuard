
#include "configuration.h"


static fnCode_type currentScreenState;
char missingNames[8][9];
u8 screen_listSize;

void Screen_Initialize (void){
   currentScreenState = screenIdle;
   screen_listSize = 0;
   ScreenUpdateAlert (NULL, 0);
}

void Screen_RunActiveState (void){
  currentScreenState();
}

void screenIdle(void){
}

void screenAlert(void){
}

void ScreenUpdateAlert (char** info, int n){
  int i, j, update = 1;
#ifdef DEBUGALARM
  n = 6;
  char* test[6];
  test[0] = "test0";
  test[1] = "test1";
  test[2] = "test2";
  test[3] = "test3";
  test[4] = "test4";
  test[5] = "test5";
  info = test;
#endif
  if (n == screen_listSize){
    for (i = 0; i < n; i++){
      update = 1;
      for (j = 0; j < n; j++){
        if (!strcmp(info[i], missingNames[j])){
          update = 0;
          break;
        }
      }
      if (update == 1){
       break;
      }
    }
  }
  if (update == 0){
    return;
  }
  
  screen_listSize = n;
  LcdClearScreen();
  PixelAddressType loc;
  loc.u16PixelColumnAddress = 0;
  loc.u16PixelRowAddress = 0;
  for (i = 0; i < n;i++){
    LcdLoadString(info[i], LCD_FONT_SMALL, &loc);
    loc.u16PixelRowAddress += 10;
    strcpy(missingNames[i], info[i]);
  }
  if (n == 0){
    LcdLoadString ("Everything is fine", LCD_FONT_SMALL, &loc);
  }
}