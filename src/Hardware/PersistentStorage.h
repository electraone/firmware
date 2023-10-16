/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

/**
 * @file PersistentStorage.h
 *
 * @brief A generic interface to non-volatile data storage.
 */

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
    bool copyFile(const char *src, const char *dest);

private:
};
