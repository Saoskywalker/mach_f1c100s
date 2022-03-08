#ifndef __FATTESTER_H
#define __FATTESTER_H

#include "types_plus.h"

u8 mf_readdir(void);
u8 mf_scan_files(u8 *path);
u8 mf_scan_files2(u8 *path, char *ptr[], u8 *cnt);
u32 mf_showfree(u8 *drv);
u8 mf_fmkfs(u8 *path, u8 mode, u16 au);
u8 mf_rename(u8 *oldname, u8 *newname);
void mf_getlabel(u8 *path);
void mf_setlabel(u8 *path);
u8 mf_copy(u8 *psrc, u8 *pdst, u8 fwmode);
u8 mf_dcopy(u8 *psrc, u8 *pdst, u8 fwmode);
u8 mf_exist_files(u8 *path, const u8 *name);
int mf_load_files(char **out, long *outsize, const char *filename);
u8 exf_getfree(u8 *drv, u32 *total, u32 *free); //得到磁盘总容量和剩余容量

#endif
