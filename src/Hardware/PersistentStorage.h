#pragma once

#include "FileIoStream.h"

class PersistentStorage : virtual private SdFs
{
public:
    PersistentStorage();
    ~PersistentStorage() = default;

    void mount(void);
    bool createDirectory(const char *path, bool pFlag = true);
    bool exists(const char *path);
    bool directoryExists(const char *path);
    FileIoStream createInputStream(const char *path);
    FileIoStream createOutputStream(const char *path, oflag_t oflag = O_RDONLY);
    bool deleteFile(const char *path);
    bool deleteRecursively(const char *path);
    bool renameFile(const char *src, const char *dest);
    bool format(void);

private:
};
