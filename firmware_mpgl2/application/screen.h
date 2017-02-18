#ifndef screen
#define screen

void Screen_Initialize(void);
void Screen_RunActiveState(void);

void screenIdle(void);
void screenAlert(void);

void ScreenUpdateAlert(u8** info, int n);
#endif