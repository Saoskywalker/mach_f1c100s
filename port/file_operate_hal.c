#include "file_operate.h" 
#include "MTF_io.h"
#include "malloc.h"		  
#include "string.h"

// #define file_operate_debug(...) printf(__VA_ARGS__)
#define file_operate_debug(...)

static FILINFO fileinfo;	//文件信息

//为磁盘注册工作区	 
//path:磁盘路径，比如"0:"、"1:"
//mt:0，不立即注册（稍后注册）；1，立即注册
//返回值:执行结果
// u8 mf_mount(u8* path,u8 mt)
// {		   
// 	return f_mount(&fs[1],(const TCHAR*)path,mt); 
// }

//打读取文件夹
//返回值:执行结果
u8 mf_readdir(void)
{
	DIR dir; //目录
	u8 res;
	char *fn;			 
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = malloc(fileinfo.lfsize);
#endif		  
	res=f_readdir(&dir,&fileinfo);//读取一个文件的信息
	if(res!=FR_OK||fileinfo.fname[0]==0)
	{
		free(fileinfo.lfname);
		return res;//读完了.
	}
#if _USE_LFN
	fn=*fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
	fn=fileinfo.fname;;
#endif	
	file_operate_debug("\r\n DIR info:\r\n");

	file_operate_debug("dir.id:%d\r\n",dir.id);
	file_operate_debug("dir.index:%d\r\n",dir.index);
	file_operate_debug("dir.sclust:%d\r\n",dir.sclust);
	file_operate_debug("dir.clust:%d\r\n",dir.clust);
	file_operate_debug("dir.sect:%d\r\n",dir.sect);	  

	file_operate_debug("\r\n");
	file_operate_debug("File Name is:%s\r\n",fn);
	file_operate_debug("File Size is:%d\r\n",fileinfo.fsize);
	file_operate_debug("File data is:%d\r\n",fileinfo.fdate);
	file_operate_debug("File time is:%d\r\n",fileinfo.ftime);
	file_operate_debug("File Attr is:%d\r\n",fileinfo.fattrib);
	file_operate_debug("\r\n");
	free(fileinfo.lfname);
	return 0;
}			 

 //遍历文件
 //path:路径
 //返回值:执行结果
u8 mf_scan_files(u8 * path)
{
	DIR dir; //目录
	FRESULT res;	  
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = malloc(fileinfo.lfsize);
#endif		  

    res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录
    if (res == FR_OK) 
	{	
		file_operate_debug("\r\n"); 
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //读取目录下的一个文件
	        if (res != FR_OK || fileinfo.fname[0] == 0) break;  //错误了/到末尾了,退出
	        //if (fileinfo.fname[0] == '.') continue;             //忽略上级目录
#if _USE_LFN
        	fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else							   
        	fn = fileinfo.fname;
#endif	                                              /* It is a file. */
			file_operate_debug("%s/", path);//打印路径	
			file_operate_debug("%s\r\n",  fn);//打印文件名	  
		} 
		f_closedir(&dir);
    }
	free(fileinfo.lfname);
    return res;	  
}

 //遍历文件到内存
 //path: 路径, ptr: 存放数组
 //返回值:执行结果
u8 mf_scan_files2(u8 * path, char *ptr[], u8 *cnt)
{
	int i = 0, j = 0;
	char *temp = ptr[0];
	DIR dir; //目录
	FRESULT res;	  
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = malloc(fileinfo.lfsize);
#endif		  

    res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录
    if (res == FR_OK) 
	{	
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //读取目录下的一个文件
	        if (res != FR_OK || fileinfo.fname[0] == 0) break;  //错误了/到末尾了,退出
	        //if (fileinfo.fname[0] == '.') continue;             //忽略上级目录
#if _USE_LFN
        	fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else							   
        	fn = fileinfo.fname;
#endif	                                              /* It is a file. */
			ptr[j] = temp;
			sprintf(ptr[j], "%s", fn);//存放路径和文件名
			// file_operate_debug("%s\r\n", ptr[j]);	
			i = strlen(ptr[j]);
			temp += i;
			temp++;
			j++;
			*cnt = j;
		} 
		f_closedir(&dir);
    }	  
	free(fileinfo.lfname);
    return res;	  
}

//显示剩余容量
//drv:盘符
//返回值:剩余容量(字节)
u32 mf_showfree(u8 *drv)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res = f_getfree((const TCHAR*)drv,(DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect = (fs1->n_fatent - 2) * fs1->csize;//得到总扇区数
	    fre_sect = fre_clust * fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		if(tot_sect<20480)//总容量小于10M
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    file_operate_debug("\r\n磁盘总容量:%d KB\r\n"
		           "可用空间:%d KB\r\n",
		           tot_sect>>1,fre_sect>>1);
		}else
		{
		    /* Print free space in unit of KB (assuming 512 bytes/sector) */
		    file_operate_debug("\r\n磁盘总容量:%d MB\r\n"
		           "可用空间:%d MB\r\n",
		           tot_sect>>11,fre_sect>>11);
		}
	}
	return fre_sect;
}		    

//格式化
//path:磁盘路径，比如"0:"、"1:"
//mode:模式
//au:簇大小
//返回值:执行结果
/* u8 mf_fmkfs(u8* path,u8 mode,u16 au)
{
	return f_mkfs((const TCHAR*)path,mode,au);//格式化,drv:盘符;mode:模式;au:簇大小
}  */

//修改文件/目录名字(如果目录不同,还可以移动文件哦!)
//oldname:之前的名字
//newname:新名字
//返回值:执行结果
/* u8 mf_rename(u8 *oldname,u8* newname)
{
	return  f_rename((const TCHAR *)oldname,(const TCHAR *)newname);
} */

//获取盘符（磁盘名字）
//path:磁盘路径，比如"0:"、"1:"  
void mf_getlabel(u8 *path)
{
	u8 buf[20];
	u32 sn=0;
	u8 res;
	res=f_getlabel ((const TCHAR *)path,(TCHAR *)buf,(DWORD*)&sn);
	if(res==FR_OK)
	{
		file_operate_debug("\r\n磁盘%s 的盘符为:%s\r\n",path,buf);
		file_operate_debug("磁盘%s 的序列号:%X\r\n\r\n",path,sn); 
	}else file_operate_debug("\r\n获取失败，错误码:%X\r\n",res);
}

//设置盘符（磁盘名字），最长11个字符！！，支持数字和大写字母组合以及汉字等
//path:磁盘号+名字，比如"0:ALIENTEK"、"1:OPENEDV"  
void mf_setlabel(u8 *path)
{
	u8 res;
	res=f_setlabel ((const TCHAR *)path);
	if(res==FR_OK)
	{
		file_operate_debug("\r\n磁盘盘符设置成功:%s\r\n",path);
	}else file_operate_debug("\r\n磁盘盘符设置失败，错误码:%X\r\n",res);
} 

//文件复制(需用的malloc)
//将psrc文件,copy到pdst.
//psrc,pdst:源文件和目标文件
//fwmode:文件写入模式
//0:不覆盖原有的文件
//1:覆盖原有的文件
u8 mf_copy(u8 *psrc, u8 *pdst, u8 fwmode)
{
	#define FATFS_COPY_ONCE_SIZE 4096 //等于存储芯片单位刷写大小或成倍数关系, 写入才快速

	UINT bw = 0, br = 0;
	FIL *fsrc = 0;
	FIL *fdst = 0;
	u8 *fbuf = 0;
	u8 res = 0;
	u32 total, freeStorage = 0;
	u8 dp[] = {0, ':', 0};
	
	dp[0] = pdst[0];
    exf_getfree(&dp[0],&total,&freeStorage); //获取磁盘容量情况
	total *= 1024; //KB->B
	freeStorage *= 1024;

	fsrc = (FIL *)malloc(sizeof(FIL)); //申请内存
	fdst = (FIL *)malloc(sizeof(FIL));
	fbuf = (u8 *)malloc(FATFS_COPY_ONCE_SIZE);
	if (fsrc == NULL || fdst == NULL || fbuf == NULL)
	{
		res = 100; //前面的值留给fatfs
	}
	else
	{
		if (fwmode == 0)
			fwmode = FA_CREATE_NEW; //不覆盖
		else
			fwmode = FA_CREATE_ALWAYS; //覆盖存在的文件

		res = f_open(fsrc, (const TCHAR *)psrc, FA_READ | FA_OPEN_EXISTING); //打开只读文件
		if (res == 0)
		{
			// file_operate_debug("storage: total %dB, free %dB, size %dB\r\n", total, freeStorage, f_size(fsrc));
			res = f_open(fdst, (const TCHAR *)pdst, FA_READ); //只读打开存在文件
			if(res==0) //存在
			{
				if(f_size(fsrc)<=freeStorage+f_size(fdst)-32*1024) //保留32k
				{
					f_close(fdst); //先关闭
					res = f_open(fdst, (const TCHAR *)pdst, 
								FA_WRITE | fwmode); //第一个打开成功,才开始打开第二个
				}
				else
				{
					res = FR_DENIED; //磁盘满
				}
			}
			else if(f_size(fsrc)<=freeStorage-32*1024) //确认磁盘有空间, 保留32k
			{
				res = f_open(fdst, (const TCHAR *)pdst, 
							FA_WRITE | fwmode); //第一个打开成功,才开始打开第二个
			}
			else
			{
				res = FR_DENIED; //磁盘满
			}
		
			if (res == 0)					//两个都打开成功了
			{
				while (res == 0) //开始复制
				{
					res = f_read(fsrc, fbuf, FATFS_COPY_ONCE_SIZE, &br); //源头读出
					if (res || br == 0)
						break;
					res = f_write(fdst, fbuf, br, &bw); //写入目的文件
					if (res || bw < br)
						break;
				}
				f_close(fsrc);
				f_close(fdst);
			}
			else
			{
				f_close(fsrc); //第二个打开失败,关闭第一个后退出
			}
		}
	}
	free(fsrc); //释放内存
	free(fdst);
	free(fbuf);
	return res;
}

//得到路径下的文件夹
//返回值:0,路径就是个卷标号.
//    其他,文件夹名字首地址
static u8 *get_src_dname(u8 *dpfn)
{
	u16 temp = 0;
	while (*dpfn != 0)
	{
		dpfn++;
		temp++;
	}
	if (temp < 4)
		return 0;
	while ((*dpfn != 0x5c) && (*dpfn != 0x2f))
		dpfn--; //追述到倒数第一个"\"或者"/"处
	return ++dpfn;
}

//文件夹复制(需用到malloc)
//将psrc文件夹,copy到pdst文件夹.
//pdst:必须形如"X:"/"X:XX"/"X:XX/XX"之类的.而且要实现确认上一级文件夹存在
//psrc,pdst:源文件夹和目标文件夹
//fwmode:文件写入模式
//0:不覆盖原有的文件
//1:覆盖原有的文件
u8 mf_dcopy(u8 *psrc, u8 *pdst, u8 fwmode)
{
#define MAX_PATHNAME_DEPTH 512 + 1 //最大目标文件路径+文件名深度
	u8 res = 0;
	DIR *srcdir = 0;	//源目录
	DIR *dstdir = 0;	//源目录
	FILINFO *finfo = 0; //文件信息
	u8 *fn = 0;			//长文件名

	u8 *dstpathname = 0; //目标文件夹路径+文件名
	u8 *srcpathname = 0; //源文件夹路径+文件名

	u16 dstpathlen = 0; //目标路径长度
	u16 srcpathlen = 0; //源路径长度

	srcdir = (DIR *)malloc(sizeof(DIR)); //申请内存
	dstdir = (DIR *)malloc(sizeof(DIR));
	finfo = (FILINFO *)malloc(sizeof(FILINFO));

	if (srcdir == NULL || dstdir == NULL || finfo == NULL)
		res = 100;
	if (res == 0)
	{
		finfo->lfsize = _MAX_LFN * 2 + 1;
		finfo->lfname = malloc(finfo->lfsize); //申请内存
		dstpathname = malloc(MAX_PATHNAME_DEPTH);
		srcpathname = malloc(MAX_PATHNAME_DEPTH);
		if (finfo->lfname == NULL || dstpathname == NULL || srcpathname == NULL)
			res = 101;
		if (res == 0)
		{
			dstpathname[0] = 0;
			srcpathname[0] = 0;
			strcat((char *)srcpathname, (const char *)psrc); //复制原始源文件路径
			strcat((char *)dstpathname, (const char *)pdst); //复制原始目标文件路径
			res = f_opendir(srcdir, (const TCHAR *)psrc);	//打开源目录
			if (res == FR_OK)									 //打开目录成功
			{
				strcat((char *)dstpathname, (const char *)"/"); //加入斜杠
				fn = get_src_dname(psrc);
				if (fn == 0) //卷标拷贝
				{
					dstpathlen = strlen((const char *)dstpathname);
					dstpathname[dstpathlen] = psrc[0]; //记录卷标
					dstpathname[dstpathlen + 1] = 0;   //结束符
				}
				else
					strcat((char *)dstpathname, (const char *)fn); //加文件名
				res = f_mkdir((const TCHAR *)dstpathname);		   //如果文件夹已经存在,就不创建.如果不存在就创建新的文件夹.
				if (res == FR_EXIST)
					res = 0;
				while (res == 0) //开始复制文件夹里面的东东
				{
					res = f_readdir(srcdir, finfo); //读取目录下的一个文件
					if (res != FR_OK || finfo->fname[0] == 0)
						break; //错误了/到末尾了,退出
					if (finfo->fname[0] == '.')
						continue;												//忽略上级目录
					fn = (u8 *)(*finfo->lfname ? finfo->lfname : finfo->fname); //得到文件名
					dstpathlen = strlen((const char *)dstpathname);				//得到当前目标路径的长度
					srcpathlen = strlen((const char *)srcpathname);				//得到源路径长度

					strcat((char *)srcpathname, (const char *)"/"); //源路径加斜杠
					if (finfo->fattrib & 0X10)						//是子目录   文件属性,0X20,归档文件;0X10,子目录;
					{
						strcat((char *)srcpathname, (const char *)fn);					  //源路径加上子目录名字
						file_operate_debug("\r\ncopy folder %s to %s\r\n", srcpathname, dstpathname); //拷贝文件
						res = mf_dcopy(srcpathname, dstpathname, fwmode);				  //拷贝文件夹
					}
					else //非目录
					{
						strcat((char *)dstpathname, (const char *)"/");					//目标路径加斜杠
						strcat((char *)dstpathname, (const char *)fn);					//目标路径加文件名
						strcat((char *)srcpathname, (const char *)fn);					//源路径加文件名
						file_operate_debug("\r\ncopy file %s to %s\r\n", srcpathname, dstpathname); //拷贝文件
						mf_copy(srcpathname, dstpathname, fwmode);						//复制文件
					}
					srcpathname[srcpathlen] = 0; //加入结束符
					dstpathname[dstpathlen] = 0; //加入结束符
				}
				f_closedir(srcdir);
			}
			free(dstpathname);
			free(srcpathname);
			free(finfo->lfname);
		}
	}
	free(srcdir);
	free(dstdir);
	free(finfo);
	return res;
}

 //查找一个文件是否存在
 //path:路径, nanme: 文件名
 //返回值:执行结果 0:存在, 其他:不存在
u8 mf_exist_files(u8 * path, const u8 *name)
{
	DIR dir; //目录
	FRESULT res;	  
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
 	fileinfo.lfsize = _MAX_LFN * 2 + 1;
	fileinfo.lfname = malloc(fileinfo.lfsize);
#endif		  

    res = f_opendir(&dir,(const TCHAR*)path); //打开一个目录
    if (res == FR_OK) 
	{	
		while(1)
		{
	        res = f_readdir(&dir, &fileinfo);                   //读取目录下的一个文件
	        if (res != FR_OK || fileinfo.fname[0] == 0) 
			{
				res = FR_NO_FILE; //不存在
				break;  //错误了/到末尾了,退出
			}
	        //if (fileinfo.fname[0] == '.') continue;             //忽略上级目录
#if _USE_LFN
        	fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else							   
        	fn = fileinfo.fname;
#endif	                                              /* It is a file. */
			// file_operate_debug("%s/", path);//打印路径	
			// file_operate_debug("%s\r\n",  fn);//打印文件名	  
			if(strcmp((const char *)name, fn)==0)
			{
				res = FR_OK; //存在
				
				break; //找到
			}
		} 
		f_closedir(&dir);
    }	  
	free(fileinfo.lfname);
    return res;	  
}

int mf_load_files(char **out, long *outsize, const char *filename)
{
	mFILE *f_temp;
	int res = 0;
	size_t size = 0, br = 0;

	f_temp = MTF_open(filename, "rb");
	if (f_temp != NULL)
	{
		size = MTF_size(f_temp);
		*out = malloc(size);
		br = MTF_read(*out, 1, size, f_temp);
		if (size != br) //读取失败
		{
			free(*out);
			*out = NULL;
			res = 7;
		}
		else
		{
			res = 0;
		}
		MTF_close(f_temp);
	}
	else
	{
		res = 4;
	}
	return res;
}

//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量	 （单位KB）
//free:剩余容量	 （单位KB）
//返回值:0,正常.其他,错误代码
u8 exf_getfree(u8 *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB 
 	}
	return res;
}	
