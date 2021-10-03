#pragma once

#include <iostream>

class File
{
    struct MultiTarget {
        const char *str; // string you're searching for
        size_t len; // length of string you're searching for
        size_t index; // index used by the search routine.
    };

public:
    File();
    File(FILE *pFile);
    bool operator!();
    void setTimeout(int timeout);
    void close();
    bool seek(int position);
    int position();
    char read();
    bool find(const char *target);
    bool find(const char *target, size_t length);
    bool findUntil(const char *target, const char *terminator);
    bool findUntil(const char *target,
                   size_t targetLen,
                   const char *terminator,
                   size_t termLen);
    int findMulti(struct MultiTarget *targets, int tCount);

private:
    FILE *pFile;
};

class SdFs
{
public:
    File open(const char *filepath);
    void mkdir(const char *filepath);
};

extern SdFs SD;
