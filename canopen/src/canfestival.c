#include "canfestival.h"

void (*initCanTimer)(void) = NULL;
void (*clearCanTimer)(void) = NULL;
void (*canTimerCallback)(void) = NULL;

unsigned char (*canInit)(CO_Data * d, uint32_t bitrate) = NULL;
unsigned char (*canSend)(CAN_PORT notused, Message *m) = NULL;
void (*canClose)(void) = NULL;

void (*setTimer)(TIMEVAL value) = NULL;
TIMEVAL (*getElapsedTime)(void) = NULL;
