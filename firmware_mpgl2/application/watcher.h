#ifndef watcher
#define watcher
unsigned char* Watcher_GetAlertStatus(void);

void Watcher_Initialize(void);
void Watcher_RunActiveState(void);

void watcherAlone(void);
void watcherWatch(void);
void watcherAlert(void);

#endif