
#pragma once

#include <sys/sys_types.h>

bool FS_Open(int* fd, const char* path);
bool FS_Close(int fd);
bool FS_Exist(const char* path);
bool FS_Read(const char* path, void* buffer, size_t len, size_t offset);
bool FS_Write(const char* path, const void* buffer, size_t len, size_t offset);

bool FS_DirExist(const char* path);
bool FS_CreateDir(const char* path);