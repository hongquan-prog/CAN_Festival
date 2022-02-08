#include "applicfg.h"
#include "data.h"
#include "TestMaster.h"

extern void (*initCanTimer)(void);
extern void (*clearCanTimer)(void);
extern void (*canTimerCallback)(void);
extern unsigned char (*canTimerEnterMutex)(void);
extern unsigned char (*canTimerLeaveMutex)(void);

extern unsigned char (*canInit)(CO_Data * d, uint32_t bitrate);
extern unsigned char (*canSend)(CAN_PORT notused, Message *m);
extern void (*canClose)(void);
