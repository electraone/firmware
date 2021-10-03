#include "SdFat.h"
#include <cstring>
#include "helpers.h"

SdFs SD;

File::File() : pFile(nullptr)
{
}

File::File(FILE *pFile) : pFile(pFile)
{
}

bool File::operator!()
{
    return (false);
}

void File::setTimeout(int timeout)
{
}

void File::close()
{
    fclose(pFile);
}

bool File::seek(int position)
{
    int rc = fseek(pFile, position, SEEK_SET);

    return (rc == 0 ? true : false);
}

int File::position()
{
    return (ftell(pFile));
}

char File::read()
{
    return (getc(pFile));
}

bool File::find(const char *target)
{
    return (findUntil(target, strlen(target), NULL, 0));
}

bool File::find(const char *target, size_t length)
{
    return (findUntil(target, length, NULL, 0));
}

bool File::findUntil(const char *target, const char *terminator)
{
    return (findUntil(target, strlen(target), terminator, strlen(terminator)));
}

bool File::findUntil(const char *target,
                     size_t targetLen,
                     const char *terminator,
                     size_t termLen)
{
    if (terminator == NULL) {
        MultiTarget t[1] = { { target, targetLen, 0 } };
        return (findMulti(t, 1) == 0 ? true : false);
    } else {
        MultiTarget t[2] = { { target, targetLen, 0 },
                             { terminator, termLen, 0 } };
        return (findMulti(t, 2) == 0 ? true : false);
    }
}

int File::findMulti(struct MultiTarget *targets, int tCount)
{
    // any zero length target string automatically matches and would make
    // a mess of the rest of the algorithm.
    for (struct MultiTarget *t = targets; t < targets + tCount; ++t) {
        if (t->len <= 0) {
            return (t - targets);
        }
    }

    while (1) {
        int c = fgetc(pFile);
        if (c < 0) {
            return (-1);
        }

        for (struct MultiTarget *t = targets; t < targets + tCount; ++t) {
            // the simple case is if we match, deal with that first.
            if (c == t->str[t->index]) {
                if (++t->index == t->len) {
                    return (t - targets);
                } else {
                    continue;
                }
            }

            // if not we need to walk back and see if we could have matched further
            // down the stream (ie '1112' doesn't match the first position in '11112'
            // but it will match the second position so we can't just reset the current
            // index to 0 when we find a mismatch.
            if (t->index == 0) {
                continue;
            }

            int origIndex = t->index;
            do {
                --t->index;
                // first check if current char works against the new current index
                if (c != t->str[t->index]) {
                    continue;
                }

                // if it's the only char then we're good, nothing more to check
                if (t->index == 0) {
                    t->index++;
                    break;
                }

                // otherwise we need to check the rest of the found string
                int diff = origIndex - t->index;
                size_t i;
                for (i = 0; i < t->index; ++i) {
                    if (t->str[i] != t->str[i + diff]) {
                        break;
                    }
                }

                // if we successfully got through the previous loop then our current
                // index is good.
                if (i == t->index) {
                    t->index++;
                    break;
                }

                // otherwise we just try the next index
            } while (t->index);
        }
    }
    // unreachable
    return (-1);
}

File SdFs::open(const char *filepath)
{
    FILE *pFile;

    pFile = fopen(filepath, "r");

    if (pFile == NULL) {
        return (File());
    }

    return (File(pFile));
}

void SdFs::mkdir(const char *filepath)
{
}
