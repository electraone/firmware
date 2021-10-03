#pragma once

#include "Hardware.h"
#include "MemoryBlock.h"

class LocalFile
{
public:
    LocalFile();
    LocalFile(const char *filepath);
    ~LocalFile();

    void setFilepath(const char *newFilepath);
    const char *getFilepath(void);

    bool createDirectory(void) ;
    bool exists(void) ;
    FileIoStream createInputStream(void);
    FileIoStream createOutputStream(oflag_t oflag = O_WRITE | O_CREAT | O_TRUNC);
    bool deleteFile(void) ;
    bool deleteRecursively(void) ;
    bool writeAll(MemoryBlock &memoryBlock);
    bool printToConsole(void);;

private:
    const char *filepath;
};
