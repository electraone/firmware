#include "PersistentStorage.h"
#include "helpers.h"

PersistentStorage::PersistentStorage()
{
}

void PersistentStorage::mount(void)
{
    while (!begin(SdioConfig(FIFO_SDIO))) {
        logMessage("Waiting for internal storage to initialize");
        delay(200);
    }
    logMessage("internal storage: Initialized");

    delay(100);
}

bool PersistentStorage::createDirectory(const char *path, bool pFlag)
{
    return (SdFs::mkdir(path, pFlag));
}

bool PersistentStorage::exists(const char *path)
{
    return (SdFs::exists(path));
}

FileIoStream PersistentStorage::createInputStream(const char *path)
{
    return (SdFs::open(path, O_RDONLY));
}

FileIoStream PersistentStorage::createOutputStream(const char *path,
                                                   oflag_t oflag)
{
    return (SdFs::open(path, oflag));
}

bool PersistentStorage::deleteFile(const char *path)
{
    return (SdFs::remove(path));
}

bool PersistentStorage::deleteRecursively(const char *path)
{
    return (SdFs::rmdir(path));
}
