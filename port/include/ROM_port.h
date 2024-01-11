/*
Copyright (c) 2019-2023 Aysi 773917760@qq.com. All right reserved
Official site: www.mtf123.club

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

It under the terms of the Apache as published;
either version 2 of the License,
or (at your option) any later version.
*/

#ifndef _ROM_PORT_H
#define _ROM_PORT_H

#include "types_base.h"

char MTF_ROM_init(void);
uint64_t MTF_unique_id(void); /*64bit唯一ID*/
char MTF_ROM_read(uint8_t *pBuffer, size_t ReadAddr, size_t NumByteToRead); //读取flash
char MTF_ROM_write(uint8_t *pBuffer, size_t WriteAddr, size_t NumByteToWrite); //写入flash
char MTF_ROM_user_data_erase(void); //擦除用户数据

#endif
