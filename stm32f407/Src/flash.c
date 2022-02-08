#include "flash.h"

void FLASH_Init(void)
{
  /* Unlock the Program memory */
  HAL_FLASH_Unlock();

  /* Clear all FLASH flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGAERR | FLASH_FLAG_WRPERR);
  /* Unlock the Program memory */
  HAL_FLASH_Lock();
}

void FLASH_Enable_Read_Protection(void)
{
  FLASH_OBProgramInitTypeDef OBInit;
  
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
  
  HAL_FLASHEx_OBGetConfig(&OBInit);
  if(OBInit.RDPLevel == OB_RDP_LEVEL_0)
  {
    OBInit.OptionType = OPTIONBYTE_RDP;
    OBInit.RDPLevel = OB_RDP_LEVEL_1;
		HAL_FLASH_OB_Unlock();
    HAL_FLASH_Unlock();
    
    HAL_FLASHEx_OBProgram(&OBInit);
		HAL_FLASH_OB_Launch();
    
    HAL_FLASH_Lock();
		HAL_FLASH_OB_Lock();
  }
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
}

void FLASH_Disable_Read_Protection(void)
{
  FLASH_OBProgramInitTypeDef OBInit;
  
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
  
  HAL_FLASHEx_OBGetConfig(&OBInit);
  if(OBInit.RDPLevel == OB_RDP_LEVEL_1)
  {
    OBInit.OptionType = OPTIONBYTE_RDP;
    OBInit.RDPLevel = OB_RDP_LEVEL_0;
		HAL_FLASH_OB_Unlock();
    HAL_FLASH_Unlock();
    
    HAL_FLASHEx_OBProgram(&OBInit);
		HAL_FLASH_OB_Launch();
    
    HAL_FLASH_Lock();
		HAL_FLASH_OB_Lock();
  }
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
}

void Flash_Enable_Write_Protection(void)
{
  FLASH_OBProgramInitTypeDef OBInit;
  
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
  
  HAL_FLASHEx_OBGetConfig(&OBInit);

	OBInit.OptionType = OPTIONBYTE_WRP;
	OBInit.WRPState=OB_WRPSTATE_ENABLE;
	OBInit.Banks=FLASH_BANK_1;
	OBInit.WRPSector=0xfff;
	
	HAL_FLASH_OB_Unlock();
	HAL_FLASH_Unlock();
	
	HAL_FLASHEx_OBProgram(&OBInit);
	HAL_FLASH_OB_Launch();
	
	HAL_FLASH_Lock();
	HAL_FLASH_OB_Lock();
 
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
}

void Flash_Disable_Write_Protection(void)
{
  FLASH_OBProgramInitTypeDef OBInit;
  
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
  
  HAL_FLASHEx_OBGetConfig(&OBInit);

	OBInit.OptionType = OPTIONBYTE_WRP;
	OBInit.WRPState=OB_WRPSTATE_DISABLE;
	OBInit.Banks=FLASH_BANK_1;
	OBInit.WRPSector=OB_WRP_SECTOR_All;
	
	HAL_FLASH_OB_Unlock();
	HAL_FLASH_Unlock();
	
	HAL_FLASHEx_OBProgram(&OBInit);
	HAL_FLASH_OB_Launch();
	
	HAL_FLASH_Lock();
	HAL_FLASH_OB_Lock();
 
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
}

uint32_t FLASH_Erase(uint32_t start_sector, uint32_t sectors)
{
	uint32_t PageError;
	
	/*erase page*/
	FLASH_EraseInitTypeDef FLASH_EraseInitStructure;
	FLASH_EraseInitStructure.NbSectors=sectors;
	FLASH_EraseInitStructure.Sector=start_sector;
	FLASH_EraseInitStructure.TypeErase=FLASH_TYPEERASE_SECTORS;
	FLASH_EraseInitStructure.VoltageRange=FLASH_VOLTAGE_RANGE_3;
	
	/*disable write protection*/
	Flash_Disable_Write_Protection();
	/*erase sector*/
	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(&FLASH_EraseInitStructure, &PageError);
	HAL_FLASH_Lock();
	return PageError;
}

int32_t FLASH_Write(uint32_t destination, uint8_t *p_source, uint32_t length)
{
  uint32_t i = 0;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  for (i = 0; i < length; i++)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, destination, *(uint8_t*)(p_source+i)) == HAL_OK)      
    {
     /* Check the written value */
      if (*(uint8_t*)destination != *(uint8_t*)(p_source+i))
      {
        /* Flash content doesn't match SRAM content */
        return FLASH_WRITING_ERROR;
      }
      /* Increment FLASH destination address */
      destination += 1;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return FLASH_WRITING_ERROR;
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  return FLASH_OK;
}

int32_t FLASH_Read(uint32_t destination, uint8_t *p_buf, uint32_t length)
{
	uint32_t i = 0;
	
	for(i = 0; i < length; i++)
	{
		p_buf[i] = (*(uint8_t*)(destination+i));
	}
	return i;
}
