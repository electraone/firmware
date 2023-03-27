#include "LocalFile.h"
#include "System.h"

LocalFile::LocalFile() : filepath("")
{
}

LocalFile::LocalFile(const char *filepath) : filepath(filepath)
{
}

LocalFile::~LocalFile()
{
}

void LocalFile::setFilepath(const char *newFilepath)
{
    filepath = newFilepath;
}

const char *LocalFile::getFilepath(void)
{
    return (filepath);
}

bool LocalFile::createDirectory(void)
{
    return (Hardware::sdcard.createDirectory(filepath));
}

bool LocalFile::exists(void)
{
    return (Hardware::sdcard.exists(filepath));
}

FileIoStream LocalFile::createInputStream(void)
{
    return (Hardware::sdcard.createInputStream(filepath));
}

FileIoStream LocalFile::createOutputStream(oflag_t oflag)
{
    return (Hardware::sdcard.createOutputStream(filepath, oflag));
}

bool LocalFile::deleteFile(void)
{
    return (Hardware::sdcard.deleteFile(filepath));
}

bool LocalFile::deleteRecursively(void)
{
    return (Hardware::sdcard.deleteRecursively(filepath));
}

bool LocalFile::writeAll(MemoryBlock &memoryBlock)
{
    static const size_t bufferSize = 200;
    File file;

    file = Hardware::sdcard.createOutputStream(filepath,
                                               FILE_WRITE | O_CREAT | O_TRUNC);

    if (!file) {
        System::logger.write(
            LOG_ERROR, "Localfile: Cannot open file failed: %s", filepath);
        return (false);
    }

    uint8_t buffer[bufferSize];
    size_t length = 0;

    while ((length = memoryBlock.readBytes(buffer, bufferSize)) > 0) {
        //logData ((uint8_t *) buffer, length);
        file.write(buffer, length);
    }

    file.close();

    return (true);
}

bool LocalFile::printToConsole(void)
{
    File file;

    file = Hardware::sdcard.createInputStream(filepath);

    if (!file) {
        Serial.print("Read file failed: ");
        Serial.println(filepath);
        return (false);
    }

    while (file.available()) {
        //Serial.write(file.read());
    }
    file.close();

    return (true);
}

bool LocalFile::rename(const char *newFilepath)
{
    File file = Hardware::sdcard.createOutputStream(filepath);

    if (!file) {
        return (false);
    }

    bool rc = file.rename(newFilepath);

    file.close();

    return rc;
}
