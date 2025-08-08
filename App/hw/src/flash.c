#include "flash.h"



#ifdef _USE_HW_FLASH
#include "cli.h"

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base address of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base address of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base address of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base address of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base address of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base address of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base address of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base address of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base address of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base address of Sector10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base address of Sector11, 128 Kbytes */


static void cliCmd(cli_args_t *args);
static uint32_t GetSector(uint32_t Address);

static bool is_init = false;



bool flashInit(void)
{

  cliAdd("flash", cliCmd);

  is_init = true;
  return is_init;
}

bool flashErase(uint32_t addr, uint32_t length)
{
  bool                   ret           = true;
  uint32_t               first_sector  = 0;
  uint32_t               nb_of_sectors = 0;
  uint32_t               sector_error  = 0;
  FLASH_EraseInitTypeDef erase_init;


  HAL_FLASH_Unlock();

  first_sector  = GetSector(addr);
  nb_of_sectors = GetSector(addr + length) - first_sector + 1;

  erase_init.TypeErase     = FLASH_TYPEERASE_SECTORS;
  erase_init.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
  erase_init.Sector        = first_sector;
  erase_init.NbSectors     = nb_of_sectors;


  ret = true;
  if(HAL_FLASHEx_Erase(&erase_init, &sector_error) != HAL_OK)
  {
    ret = false;
  }

  HAL_FLASH_Lock();

  return ret;
}

bool flashRead(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  memcpy(p_data, (void *)addr, length);
  return true;
}

bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;

  HAL_FLASH_Unlock();

  for (uint32_t i=0; i<length; i++)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr + i, p_data[i]) != HAL_OK)
    {
      ret = false;
      break;
    }
  }

  HAL_FLASH_Lock();

  return ret;
}

uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;

  if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;
  }
  else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;
  }
  else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;
  }
  else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;
  }
  else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;
  }
  else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;
  }
  else if ((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;
  }
  else if ((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;
  }
  else if ((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;
  }
  else if ((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;
  }
  else if ((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;
  }
  else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11) */
  {
    sector = FLASH_SECTOR_11;
  }
  return sector;
}

void cliCmd(cli_args_t *args)
{
  bool ret = false;


  if (args->argc == 3 && args->isStr(0, "info"))
  {
    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "read"))
  {
    uint32_t addr;
    uint32_t length;

    addr   = args->getData(1);
    length = args->getData(2);

    for (int i=0; i<(int)length; i++)
    {
      uint8_t data;

      flashRead(addr + i, &data, 1);
      cliPrintf("addr 0x%08X : 0x%02X\n", addr + i, data);
    }
    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "write"))
  {
    uint32_t addr;
    uint32_t data;

    addr = args->getData(1);
    data = args->getData(2);

    if (flashWrite(addr, (uint8_t *)&data, 4))
    {
      cliPrintf("flashWrite() - OK\n");
    }
    else
    {
      cliPrintf("flashWrite() - FAIL\n");
    }
    ret = true;
  }

  if (args->argc == 3 && args->isStr(0, "erase"))
  {
    uint32_t addr;
    uint32_t length;

    addr = args->getData(1);
    length = args->getData(2);

    if (flashErase(addr, length))
    {
      cliPrintf("flashErase() - OK\n");
    }
    else
    {
      cliPrintf("flashErase() - FAIL\n");
    }
    ret = true;
  }

  if (!ret)
  {
    cliPrintf("flash info\n");
    cliPrintf("flash read addr length\n");
    cliPrintf("flash write addr data\n");
    cliPrintf("flash erase addr length\n");
  }
}

#endif