/********************************************
 * 用于平替stdio.h功能, 增加可移植性
 * http://www.mtf123.club/
 * Copyright © MTF. All Rights Reserved
********************************************/

#ifndef _MTF_IO_H
#define _MTF_IO_H

#include "stdio.h"
#include "ff.h"

//port platform

typedef FIL mFILE;

/*********文件系统定义**********/
typedef struct
{
    int id; //路径
    void *ptr; //文件系统信息
    // char kind; //挂载种类
    // char way; //挂载权限
}mMount;

int MTF_mount(mMount *dest);
extern mMount file_system_set[2];
#define FILE_SYS_SD file_system_set[0]
#define FILE_SYS_FLASH file_system_set[1]

/*****************************/

#ifndef SEEK_CUR
#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0
#endif

// #ifndef EOF
// #define EOF (-1)
// #endif

/////////////////

//新增
mFILE *cache_open(const char *filename, const char *mode);
int cache_close(mFILE *stream);
size_t cache_read(void *ptr, size_t nmemb, mFILE *stream);
size_t cache_write(const void *ptr, size_t nmemb, mFILE *stream);
long int cache_tell(mFILE *stream);
int cache_lseek(mFILE *stream, long int offset, int whence);

//file函数
int MTF_error(mFILE *stream);
mFILE *MTF_open(const char *filename, const char *mode);
int MTF_close(mFILE *stream);
size_t MTF_read(void *ptr, size_t size, size_t nmemb, mFILE *stream);
size_t MTF_write(const void *ptr, size_t size, size_t nmemb, mFILE *stream);
int MTF_seek(mFILE *stream, long int offset, int whence);
long int MTF_tell(mFILE *stream);
int MTF_remove(const char *filename);
int MTF_rename(const char *old_filename, const char *new_filename);
size_t MTF_size(mFILE *stream);
int MTF_eof(mFILE *stream);

#endif
