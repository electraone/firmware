#include "PersistentStorage.h"
#include "System.h"

PersistentStorage::PersistentStorage()
{
}

void PersistentStorage::mount(void)
{
    while (!begin(SdioConfig(FIFO_SDIO))) {
        System::logger.write(LOG_ERROR,
                             "Waiting for internal storage to initialize");
        delay(200);
    }
    System::logger.write(LOG_ERROR, "internal storage: Initialized");

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

bool PersistentStorage::directoryExists(const char *path)
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

bool PersistentStorage::renameFile(const char *src, const char *dest)
{
    return (SdFs::rename(src, dest));
}

bool PersistentStorage::format(void)
{
    return SdFs::format();
}
