
#include "file.hpp"
#include "imports.hpp"
#include <cell/cell_fs.h>

bool FS_Open(int* fd, const char* path)
{
    return cellFsOpen(path, CELL_FS_O_RDWR | CELL_FS_O_CREAT | CELL_FS_O_TRUNC, fd, 0, 0) == 0;
}

bool FS_Close(int fd)
{
    return cellFsClose(fd) == 0;
}

bool FS_Exist(const char* path)
{
    CellFsStat stat;
    if (cellFsStat(path, &stat) == 0)
    {
        return stat.st_mode & CELL_FS_S_IFREG;
    }
    return false;
}

bool FS_Read(const char* path, void* buffer, size_t len, size_t offset)
{
    int fd;
    if (cellFsOpen(path, CELL_FS_O_RDONLY, &fd, 0, 0) == 0)
    {
        uint64_t pos, read;
        cellFsLseek(fd, offset, CELL_FS_SEEK_SET, &pos);
        cellFsRead(fd, buffer, len, &read);

        FS_Close(fd);
        return true;
    }
    return false;
}

bool FS_Write(const char* path, const void* buffer, size_t len, size_t offset)
{
    int fd;
    if (FS_Open(&fd, path))
    {
        uint64_t pos, write;
        cellFsLseek(fd, offset, CELL_FS_SEEK_SET, &pos);
        cellFsWrite(fd, buffer, len, &write);

        FS_Close(fd);
        return true;
    }
    return false;
}

bool FS_DirExist(const char* path)
{
    CellFsStat stat;
    if (cellFsStat(path, &stat) == 0)
    {
        return stat.st_mode & CELL_FS_S_IFDIR;
    }
    return false;
}

bool FS_CreateDir(const char* path)
{
    const char* _path = path;

    int pathLen = _sys_strlen(path);
    char* pathStep = new char[pathLen];
    _sys_memset(pathStep, 0, pathLen);

    while (*++path)
    {
        if (*path == '/')
        {
            _sys_strncpy(pathStep, _path, path - _path);
            pathStep[path - _path] = 0;

            if (!FS_DirExist(pathStep))
            {
                cellFsMkdir((const char*)pathStep, 0777);
            }
        }
    }

    delete[] pathStep;
    return true;
}