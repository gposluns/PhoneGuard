#ifndef watcher
#define watcher

void WatcherInitialize(void);
void WatcherRunActiveState(void);
void WatcherSM_Idle(void);
void WatcherSM_FailedInit(void);

void watcherWatch(void);
void WatcherAlert(void);
void watcherNoteResponse(u8* message);
void WatcherRegister(u8* message);
void WatcherRelease(u8*message);

#define MAX_WAIT                5
#define DEFAULT_LIST_SIZE       8
#define MAX_NAME_LENGTH         7
#endif