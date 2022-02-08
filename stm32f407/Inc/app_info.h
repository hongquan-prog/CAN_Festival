#ifndef __APP_INFO_H
#define __APP_INFO_H
#include "flash.h"
#include "partition.h"

typedef enum{
	APP_1,
	APP_2
}App_TypeDef;

typedef enum{
	CMD_STOP = 0,
	CMD_START = 1,
	CMD_RESET = 2,
	CMD_CLEAR = 3,
	CMD_START_BOOTLOADER = 0x80,
	CMD_SET_SIGNATURE = 0x83,
	CMD_CLEAR_SIGNATURE = 0x84
}App_ProgramCmdTypeDef;

typedef enum{
	APP_UPDATE,
	APP_UPDATE_FINISH
}App_Updata_StatusTypeDef;

typedef struct{
	uint32_t app_size;
	uint32_t updata_status;
	uint8_t version[4];
}App_PropertyTypeDef;


uint32_t APP_Erase(App_TypeDef app);
int32_t APP_Write(App_TypeDef app, uint32_t offset, uint8_t *data, uint32_t size);

#endif
