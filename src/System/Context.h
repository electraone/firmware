#pragma once

#include <cstdio>
#include "settings.h"
#include "helpers.h"

class Context
{
public:
    Context()
        : appName(nullptr),
          presetName(nullptr),
          midiLearnEnabled(false),
          loadFiles(true),
          touchEnabled(true)
    {
    }

    void setAppName(const char *newAppName)
    {
        appName = newAppName;
        snprintf(
            currentConfigFile, MAX_FILENAME_LENGTH, "%s/setup.cfg", appName);
        snprintf(currentTempFile, MAX_FILENAME_LENGTH, "%s/file.tmp", appName);
    }

    void setPresetName(const char *newPresetName)
    {
        presetName = newPresetName;
    }

    const char *getAppName(void)
    {
        return (appName);
    }

    const char *getPresetName(void)
    {
        return (presetName);
    }

    void setMidiLearn(bool shouldBeEnabled)
    {
        midiLearnEnabled = shouldBeEnabled;
    }

    bool getMidiLearn(void)
    {
        return (midiLearnEnabled);
    }

    void setLoadDefaultFiles(bool shouldLoadFiles)
    {
        loadFiles = shouldLoadFiles;
    }

    bool getLoadDefaultFiles(void)
    {
        return (loadFiles);
    }

    void setTouchEnabled(bool shouldTouchBeEnabled)
    {
        touchEnabled = shouldTouchBeEnabled;
    }

    bool getTouchEnabled(void)
    {
        return (touchEnabled);
    }

    // \todo this is not OK
    void setCurrentFile(uint16_t presetId)
    {
        if (appName) {
            formatPresetFilename(
                currentPresetFile, MAX_FILENAME_LENGTH, presetId);
            formatLuaFilename(currentLuaFile, MAX_FILENAME_LENGTH, presetId);
        }
    }

    char *getCurrentPresetFile(void)
    {
        return (currentPresetFile);
    }

    char *getCurrentLuaFile(void)
    {
        return (currentLuaFile);
    }

    char *getCurrentConfigFile(void)
    {
        return (currentConfigFile);
    }

    char *getCurrentTempFile(void)
    {
        return (currentTempFile);
    }

    void formatPresetFilename(char *buffer, size_t maxSize, uint16_t fileNumber)
    {
        snprintf(buffer, maxSize - 1, "%s/p%03d.epr", appName, fileNumber);
    }

    void formatLuaFilename(char *buffer, size_t maxSize, uint16_t fileNumber)
    {
        snprintf(buffer, maxSize - 1, "%s/p%03d.lua", appName, fileNumber);
    }

private:
    char currentPresetFile[MAX_FILENAME_LENGTH];
    char currentLuaFile[MAX_FILENAME_LENGTH];
    char currentConfigFile[MAX_FILENAME_LENGTH];
    char currentTempFile[MAX_FILENAME_LENGTH];
    const char *appName;
    const char *presetName;

    bool midiLearnEnabled;
    bool loadFiles;
    bool touchEnabled;
};
