#include "app_info.h"




void App_Enable_Update()
{
	
}

void App_Disable_Update() 
{
	
}

int32_t App_IS_Update() 
{
	return -1;
}


int32_t App_Info_Read(App_PropertyTypeDef * property)
{
	FLASH_Read(APP_INFO_START_ADDRESS,(uint8_t *)property,sizeof(App_PropertyTypeDef));
	return 0;
}
uint32_t App_Info_Write(App_PropertyTypeDef *property)
{
	FLASH_Erase(APP_INFO_START_SECTOR, APP_INFO_TOTAL_SECTORS);
	return FLASH_Write(APP_INFO_START_ADDRESS, (uint8_t *)property, sizeof(App_PropertyTypeDef));
}

uint32_t APP_Erase(App_TypeDef app)
{
	if(APP_1 == app)
	{
		return FLASH_Erase(APP_1_START_SECTOR,APP_1_TOTAL_SECTORS);
	}
	else if(APP_2 == app)
	{
		return FLASH_Erase(APP_2_START_SECTOR,APP_2_TOTAL_SECTORS);
	}
	return 0;
}

int32_t APP_Write(App_TypeDef app, uint32_t offset, uint8_t *data, uint32_t size)
{
	if(APP_1 == app)
	{
		offset += APP_1_START_ADDRESS;
	}
	else if(APP_2 == app)
	{
		offset += APP_2_START_ADDRESS;
	}
	else
	{
		return -1;
	}
	return FLASH_Write(offset, data, size);
}
