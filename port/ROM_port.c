#include "ROM_port.h"
#include "w25qxx.h"

char MTF_ROM_init(void)
{
    W25QXX_Init();
    return 0;
}

uint64_t MTF_unique_id(void) /*64bit唯一ID*/
{
    return W25QXX_UniqueID();
}

char MTF_ROM_read(uint8_t *pBuffer, size_t ReadAddr, size_t NumByteToRead) //读取flash
{
    W25QXX_Read(pBuffer, (uint32_t)ReadAddr, (uint16_t)NumByteToRead);
    // return 1;; //fail
    return 0; //success
}

char MTF_ROM_write(uint8_t *pBuffer, size_t WriteAddr, size_t NumByteToWrite) //写入flash
{
    W25QXX_Write(pBuffer, (uint32_t)WriteAddr, (uint16_t)NumByteToWrite);
    return 0; //success
}

char MTF_ROM_user_data_erase(void) //擦除用户数据
{
    /*FLASH ADDR*/
    //擦除扇区
    W25QXX_Erase_Sector_Range(512, 4096 - 512); //FATFS区域擦书, 加快读写速度, 超慢
    // W25QXX_Erase_Chip(); //整片芯片刷除, 比删除部分扇区快
    return 0;
}
