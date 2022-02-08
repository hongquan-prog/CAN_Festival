#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f4xx.h"

/* Error code */
enum 
{
  FLASH_OK = 0,
  FLASH_ERASEKO,
  FLASH_WRITINGCTRL_ERROR,
  FLASH_WRITING_ERROR,
  FLASH_PROTECTION_ERRROR
};

/* Base address of the Flash sectors */
#define ADDR_FLASH_PAGE_0     ((uint32_t)0x08000000) /* Base @ of Page 0, 16 Kbytes */
#define ADDR_FLASH_PAGE_1     ((uint32_t)0x08004000) /* Base @ of Page 1, 16 Kbytes */
#define ADDR_FLASH_PAGE_2     ((uint32_t)0x08008000) /* Base @ of Page 2, 16 Kbytes */
#define ADDR_FLASH_PAGE_3     ((uint32_t)0x0800C000) /* Base @ of Page 3, 16 Kbytes */
#define ADDR_FLASH_PAGE_4     ((uint32_t)0x08010000) /* Base @ of Page 4, 64 Kbytes */
#define ADDR_FLASH_PAGE_5     ((uint32_t)0x08020000) /* Base @ of Page 5, 128 Kbytes */
#define ADDR_FLASH_PAGE_6     ((uint32_t)0x08040000) /* Base @ of Page 6, 128 Kbytes */
#define ADDR_FLASH_PAGE_7     ((uint32_t)0x08060000) /* Base @ of Page 7, 128 Kbytes */
#define ADDR_FLASH_PAGE_8     ((uint32_t)0x08080000) /* Base @ of Page 8, 128 Kbytes */
#define ADDR_FLASH_PAGE_9     ((uint32_t)0x080A0000) /* Base @ of Page 9, 128 Kbytes */
#define ADDR_FLASH_PAGE_10    ((uint32_t)0x080C0000) /* Base @ of Page 10, 128 Kbytes */
#define ADDR_FLASH_PAGE_11    ((uint32_t)0x080E0000) /* Base @ of Page 11, 128 Kbytes */

void FLASH_Init(void);
void FLASH_Enable_Read_Protection(void);
void FLASH_Disable_Read_Protection(void);
void Flash_Enable_Write_Protection(void);
void Flash_Disable_Write_Protection(void);
	
uint32_t FLASH_Erase(uint32_t start_sector, uint32_t sectors);
int32_t FLASH_Write(uint32_t destination, uint8_t *p_source, uint32_t length);
int32_t FLASH_Read(uint32_t destination, uint8_t *p_buf, uint32_t length);

#endif

