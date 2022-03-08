/********************************************
 * 用于平替stdio.h功能, 增加可移植性
 * http://www.mtf123.club/
 * Copyright © MTF. All Rights Reserved
********************************************/

#include "MTF_io.h"
#include "malloc.h"

/*******************************************/
 //存储缓存: 把指定文件从FLASH复制至RAM, 加快使用时访问速度, 此处针对FATFS使用
static FRESULT _t_open (
	FIL* fp,			/* Pointer to the blank file object */
	const TCHAR* path,	/* Pointer to the file name */
	BYTE mode			/* Access mode and file open mode flags */
)
{
	FRESULT res = 0;
	DWORD size = 0;
	UINT br = 0;

	res = f_open(fp, path, mode);
	fp->data = NULL;
	fp->fptr = 0;
	if(res==0)
	{
		size = f_size(fp);
		if(size<=2*1024*1024) //2MB限制
		{
			fp->data = malloc(size);
			if(fp->data) //使用缓存
			{
				// printf("cache read\r\n");
				res = f_read(fp, fp->data, size, &br);
			}
			f_lseek(fp, 0);
			if(res) //读取失败
			{
				// printf("cache read error: %d\r\n", res);
				free(fp->data);
				fp->data = NULL;
			}
			res = FR_OK;
		}
		else
		{
			res = FR_OK;
		}
	}
	return res;
}

static FRESULT _t_close (
	FIL *fp		/* Pointer to the file object to be closed */
)
{
	FRESULT res = 0;

	if(fp->data) //是否使用缓存
		free(fp->data);
		
	res = f_close(fp);
	return res;
}

static FRESULT _t_write (
	FIL* fp,			/* Pointer to the file object */
	const void *buff,	/* Pointer to the data to be written */
	UINT btw,			/* Number of bytes to write */
	UINT* bw			/* Pointer to number of bytes written */
)
{
	FRESULT res = 0;
	return res;
}

static FRESULT _t_read (
	FIL* fp, 		/* Pointer to the file object */
	void* buff,		/* Pointer to data buffer */
	UINT btr,		/* Number of bytes to read */
	UINT* br		/* Pointer to number of bytes read */
)
{
	FRESULT res = 0;
	BYTE *data = buff;
	UINT cnt = 0;

	if(fp->data) //是否使用缓存
	{
		for(UINT i = 0; i<btr; i++)
		{
			if(fp->fptr+1<=fp->fsize) //超过文件大小
			{
				*data = *(BYTE *)(fp->fptr+(DWORD)fp->data);
				data++;
				fp->fptr++;
				cnt++;
			}
			else
			{
				break;
			}
		}
		*br = cnt;
		if(cnt==0)
			res = FR_DENIED;
		else
			res = FR_OK;
	}
	else
	{
		res = f_read(fp, buff, btr, br);
	}
	return res;
}

static DWORD _t_tell(FIL* fp) 
{
	return f_tell(fp);
}

static FRESULT _t_lseek (
	FIL* fp,		/* Pointer to the file object */
	DWORD ofs		/* File pointer from top of file */
)
{
	FRESULT res = 0;

	if(fp->data) //是否使用缓存
	{
		fp->fptr = ofs;
		if(fp->fptr+1<=fp->fsize) //超过文件大小
		{
			res = FR_OK;
		}
		else
		{
			res = FR_INT_ERR;
		}
	}
	else
	{
		res = f_lseek(fp, ofs);
	}
	return res;
}

mFILE *cache_open(const char *filename, const char *mode)
{
    mFILE *file;
    FRESULT res;
    BYTE setMode = 0;

    file = (mFILE *)malloc(sizeof(mFILE));

    if (file == NULL)
        return NULL;

    /*
    ⑴文件使用方式由r,w,a,t,b，+六个字符拼成，各字符的含义是：
    r(read): 只读
    w(write): 只写
    a(append): 追加
    t(text): 文本文件，可省略不写
    b(binary): 二进制文件
    +: 读和写
    ⑵ 凡用“r”打开一个文件时，该文件必须已经存在，且只能从该文件读出。
    ⑶用“w”打开的文件只能向该文件写入。若打开的文件不存在，则以指定的文件名建立该文件，
        若打开的文件已经存在，则将该文件删去，重建一个新文件。
    ⑷ 若要向一个已存在的文件追加新的信息，用“a”方式打开文件。如果指定文件不存在则尝试创建该文件。
    ⑸ 在打开一个文件时，如果出错，fopen将返回一个空指针值NULL。在程序中可以用这一信息来判别是否完成打开文件的工作，
        并作相应的处理。因此常用以下程序段打开文件(假设文件路径已写入到name字符数组中)：
    
    FATFS:
    FA_READ 指定读访问对象。可以从文件中读取数据。
            与FA_WRITE结合可以进行读写访问。
    FA_WRITE 指定写访问对象。可以向文件中写入数据。
            与FA_READ结合可以进行读写访问。
    FA_OPEN_EXISTING 打开文件。如果文件不存在，则打开失败。(默认)
    FA_OPEN_ALWAYS 如果文件存在，则打开；否则，创建一个新文件。
    FA_CREATE_NEW 创建一个新文件。如果文件已存在，则创建失败。
    FA_CREATE_ALWAYS 创建一个新文件。如果文件已存在，则它将被截断并覆盖。
    */
    if (mode[0] == 'r' && mode[1] == 'b')
    {
        if (mode[2] == '+')
            setMode = FA_READ | FA_WRITE | FA_OPEN_EXISTING;
        else
            setMode = FA_READ | FA_OPEN_EXISTING;
    }
    else if (mode[0] == 'w' && mode[1] == 'b')
    {
        if (mode[2] == '+')
            setMode = FA_READ | FA_WRITE | FA_CREATE_ALWAYS;
        else
            setMode = FA_WRITE | FA_CREATE_ALWAYS;
    }
    else if (mode[0] == 'a' && mode[1] == 'b')
    {
        if (mode[2] == '+')
            setMode = FA_WRITE | FA_OPEN_ALWAYS;
        else
            setMode = FA_READ | FA_WRITE | FA_OPEN_ALWAYS;
    }
    else
    {
        free(file);
        return NULL;
    }

    res = _t_open(file, filename, setMode);
    if (res == FR_OK)
    {
        return file;
    }
    else
    {
        free(file);
        return NULL;
    }
}

int cache_close(mFILE *stream)
{
    FRESULT res;

    res = _t_close(stream);
    if (res == FR_OK)
    {
        free(stream);
        return 0;
    }
    else
    {
        return -1;
    }
}

size_t cache_read(void *ptr, size_t nmemb, mFILE *stream)
{
    UINT fbr;
    FRESULT res;

    res = _t_read(stream, ptr, (UINT)nmemb, &fbr);
    if(res == FR_OK)
        return (size_t)fbr;
    else
        return 0;
}

size_t cache_write(const void *ptr, size_t nmemb, mFILE *stream)
{
    UINT fbr;
    FRESULT res;

    res = _t_write(stream, ptr, (UINT)nmemb, &fbr);
    if (res == FR_OK && nmemb == fbr)
        return (size_t)fbr;
    else
        return 0;
}

long int cache_tell(mFILE *stream)
{
    return (long int)_t_tell(stream);
}

int cache_lseek(mFILE *stream, long int offset, int whence)
{
    FRESULT res = 1;
    long int i = 0;
    size_t j = 0;

    if (whence == SEEK_SET) //文件开头处偏移
    {
        res = _t_lseek(stream, (DWORD)offset);
    }
    else if (whence == SEEK_CUR) //文件指针的当前位置
    {
        i = cache_tell(stream);
        res = _t_lseek(stream, (DWORD)(i + offset));
    }
    else if (whence == SEEK_END) //文件的末尾
    {
        j = (size_t)f_size(stream) - 1;
        res = _t_lseek(stream, (DWORD)(j - offset));
    }

    if (res == FR_OK)
        return 0;
    else
        return -1;
}

/*****************************************/

/************************
 * ferror，函数名，在调用各种输入输出函数（如 putc.getc.fread.fwrite等）时，
 * 如果出现错误，除了函数返回值有所反映外，还可以用ferror函数检查。 
 * 它的一般调用形式为 ferror(fp)；如果ferror返回值为0（假），表示未出错。
 * 如果返回一个非零值，表示出错。应该注意，对同一个文件 每一次调用输入输出函数，
 * 均产生一个新的ferror函 数值，因此，应当在调用一个输入输出函数后立即检 
 * 查ferror函数的值，否则信息会丢失。在执行fopen函数时，ferror函数的初始值自动置为0。
 * **********************/
int MTF_error(mFILE *stream)
{
    return f_error(stream);
}

mFILE *MTF_open(const char *filename, const char *mode)
{
    mFILE *file;
    FRESULT res;
    BYTE setMode = 0;

    file = (mFILE *)malloc(sizeof(mFILE));

    if (file == NULL)
        return NULL;

    /*
    ⑴文件使用方式由r,w,a,t,b，+六个字符拼成，各字符的含义是：
    r(read): 只读
    w(write): 只写
    a(append): 追加
    t(text): 文本文件，可省略不写
    b(binary): 二进制文件
    +: 读和写
    ⑵ 凡用“r”打开一个文件时，该文件必须已经存在，且只能从该文件读出。
    ⑶用“w”打开的文件只能向该文件写入。若打开的文件不存在，则以指定的文件名建立该文件，
        若打开的文件已经存在，则将该文件删去，重建一个新文件。
    ⑷ 若要向一个已存在的文件追加新的信息，用“a”方式打开文件。如果指定文件不存在则尝试创建该文件。
    ⑸ 在打开一个文件时，如果出错，fopen将返回一个空指针值NULL。在程序中可以用这一信息来判别是否完成打开文件的工作，
        并作相应的处理。因此常用以下程序段打开文件(假设文件路径已写入到name字符数组中)：
    
    FATFS:
    FA_READ 指定读访问对象。可以从文件中读取数据。
            与FA_WRITE结合可以进行读写访问。
    FA_WRITE 指定写访问对象。可以向文件中写入数据。
            与FA_READ结合可以进行读写访问。
    FA_OPEN_EXISTING 打开文件。如果文件不存在，则打开失败。(默认)
    FA_OPEN_ALWAYS 如果文件存在，则打开；否则，创建一个新文件。
    FA_CREATE_NEW 创建一个新文件。如果文件已存在，则创建失败。
    FA_CREATE_ALWAYS 创建一个新文件。如果文件已存在，则它将被截断并覆盖。
    */
    if (mode[0] == 'r' && mode[1] == 'b')
    {
        if (mode[2] == '+')
            setMode = FA_READ | FA_WRITE | FA_OPEN_EXISTING;
        else
            setMode = FA_READ | FA_OPEN_EXISTING;
    }
    else if (mode[0] == 'w' && mode[1] == 'b')
    {
        if (mode[2] == '+')
            setMode = FA_READ | FA_WRITE | FA_CREATE_ALWAYS;
        else
            setMode = FA_WRITE | FA_CREATE_ALWAYS;
    }
    else if (mode[0] == 'a' && mode[1] == 'b')
    {
        if (mode[2] == '+')
            setMode = FA_WRITE | FA_OPEN_ALWAYS;
        else
            setMode = FA_READ | FA_WRITE | FA_OPEN_ALWAYS;
    }
    else
    {
        free(file);
        return NULL;
    }

    res = f_open(file, filename, setMode);
    if (res == FR_OK)
    {
        return file;
    }
    else
    {
        free(file);
        return NULL;
    }
}

int MTF_close(mFILE *stream)
{
    FRESULT res;

    if (stream == NULL)
        return -1;
    res = f_close(stream);
    if (res == FR_OK)
    {
        free(stream);
        return 0;
    }
    else
    {
        return -1;
    }
}

size_t MTF_read(void *ptr, size_t size, size_t nmemb, mFILE *stream)
{
    UINT fbr;
    FRESULT res;

    if (stream == NULL)
        return 0;
    res = f_read(stream, ptr, (UINT)nmemb, &fbr);
    if(res == FR_OK)
        return (size_t)fbr;
    else
        return 0;
}

size_t MTF_write(const void *ptr, size_t size, size_t nmemb, mFILE *stream)
{
    UINT fbr;
    FRESULT res;

    if (stream == NULL)
        return 0;
    res = f_write(stream, ptr, (UINT)nmemb, &fbr);
    if (res == FR_OK && nmemb == fbr)
        return (size_t)fbr;
    else
        return 0;
}

int MTF_seek(mFILE *stream, long int offset, int whence)
{
    FRESULT res = 1;
    long int i = 0;
    size_t j = 0;

    if (whence == SEEK_SET) //文件开头处偏移
    {
        res = f_lseek(stream, (DWORD)offset);
    }
    else if (whence == SEEK_CUR) //文件指针的当前位置
    {
        i = MTF_tell(stream);
        res = f_lseek(stream, (DWORD)(i + offset));
    }
    else if (whence == SEEK_END) //文件的末尾
    {
        j = MTF_size(stream) - 1;
        res = f_lseek(stream, (DWORD)(j - offset));
    }

    if (stream == NULL)
        return -1;
    if (res == FR_OK)
        return 0;
    else
        return -1;
}

long int MTF_tell(mFILE *stream)
{
    return (long int)f_tell(stream);
}

int MTF_remove(const char *filename)
{
    FRESULT res;

    res = f_unlink(filename);
    if (res == FR_OK)
        return 0;
    else
        return -1;
}

int MTF_rename(const char *old_filename, const char *new_filename)
{
    FRESULT res;

    res = f_rename(old_filename, new_filename);
    if (res == FR_OK)
        return 0;
    else
        return -1;

}

size_t MTF_size(mFILE *stream)
{
    return (size_t)f_size(stream);
}

/**********************
 * 返回值 当文件指针到达文件结尾时，函数返回1，否则返回0.
 * ********************/
int MTF_eof(mFILE *stream)
{
    return f_eof(stream);
}

/*******************************
 * 挂载文件系统, 包括: 文件系统初始化和存储设备初始化
 * ******************************/	
mMount file_system_set[2] = {{2, NULL}, {3, NULL}};
int MTF_mount(mMount *dest)
{
    static FATFS fatfs_sys[_VOLUMES];//逻辑磁盘工作区.
    int res = 0;

    //0, 1 id 保留
    if(dest->id==2) //SD
    {
        res = f_mount(&fatfs_sys[0], "0:", 1);
        dest->ptr = &fatfs_sys[0];
    }
    else if(dest->id==3) //FLASH
    {
        res = f_mount(&fatfs_sys[1], "1:", 1);
        dest->ptr = &fatfs_sys[1];
    }
    else
    {
        res = 1; //失败
        dest->ptr = NULL;
    }
    return res;
}
