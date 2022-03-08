#ifndef _ROM_PORT_H
#define _ROM_PORT_H

#include "types_base.h"

char MTF_ROM_init(void);
uint64_t MTF_unique_id(void); /*64bit唯一ID*/
char MTF_ROM_read(uint8_t *pBuffer, size_t ReadAddr, size_t NumByteToRead); //读取flash
char MTF_ROM_write(uint8_t *pBuffer, size_t WriteAddr, size_t NumByteToWrite); //写入flash
char MTF_ROM_user_data_erase(void); //擦除用户数据

#endif
