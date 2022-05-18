#include "Hardware.h"
#include "helpers.h"

#define SQLITE_VFS_BUFFERS 2048
#define SQLITE_DEFAULT_PAGE_SIZE 1024
#define SQLITE_MAX_DEFAULT_PAGE_SIZE 1024
#define SQLITE_DEFAULT_CACHE_SIZE 16384

#include <sqlite3.h>

#define MAXPATHNAME 128

typedef struct E1File E1File;

struct E1File {
    sqlite3_file base; // Base class. Must be first.
    FileIoStream file;
    char *aBuffer; // Pointer to malloc'd buffer
    int nBuffer; // Valid bytes of data in zBuffer
    sqlite3_int64 iBufferOfst; // Offset in file of zBuffer[0]
};

static int e1Lock(sqlite3_file *pFile, int eLock)
{
    return (SQLITE_OK);
}
static int e1Unlock(sqlite3_file *pFile, int eLock)
{
    return (SQLITE_OK);
}
static int e1CheckReservedLock(sqlite3_file *pFile, int *pResOut)
{
    *pResOut = 0;
    return (SQLITE_OK);
}

static int E1FileControl(sqlite3_file *pFile, int op, void *pArg)
{
    return (SQLITE_OK);
}

static int
    e1DirectWrite(E1File *p, const void *buffer, int size, sqlite_int64 offset)
{
    if (!(p->file.seek(offset))) {
        logMessage("e1DirectWrite: Seek error");
        return SQLITE_IOERR_WRITE;
    }

    int bytesWritten = p->file.write(buffer, size);

    if (bytesWritten != size) {
        logMessage("e1DirectWrite: Write error");
        return SQLITE_IOERR_WRITE;
    }

    return (SQLITE_OK);
}

static int e1FlushBuffer(E1File *p)
{
    int rc = SQLITE_OK;

    if (p->nBuffer) {
        rc = e1DirectWrite(p, p->aBuffer, p->nBuffer, p->iBufferOfst);
        p->nBuffer = 0;
    }

    return (rc);
}

static int e1SectorSize(sqlite3_file *pFile)
{
    return (0);
}

static int e1DeviceCharacteristics(sqlite3_file *pFile)
{
    return (0);
}

static int e1Close(sqlite3_file *pFile)
{
    E1File *p = (E1File *)pFile;

    int rc = e1FlushBuffer(p);
    sqlite3_free(p->aBuffer);
    p->file.close();

    return (rc);
}

static int
    e1Read(sqlite3_file *pFile, void *buffer, int size, sqlite_int64 offset)
{
    E1File *p = (E1File *)pFile;

    int rc = e1FlushBuffer(p);

    if (rc != SQLITE_OK) {
        return (rc);
    }

    if (p->file.seek(offset) != true) {
        //return(SQLITE_IOERR_READ); // \todo figure out why it sometimes fails
    }

    int bytesRead = p->file.read(buffer, size);

    if (bytesRead == size) {
        return (SQLITE_OK);
    } else if (bytesRead >= 0) {
        return (SQLITE_IOERR_SHORT_READ);
    }

    return (SQLITE_IOERR_READ);
}

static int e1Write(sqlite3_file *pFile,
                   const void *buffer,
                   int size,
                   sqlite_int64 offset)
{
    E1File *p = (E1File *)pFile;

    if (p->aBuffer) {
        char *tempBuffer = (char *)buffer;
        int remainingBytes = size;
        sqlite3_int64 currentPosition = offset;

        while (remainingBytes > 0) {
            int bytesToWrite;

            if (p->nBuffer == SQLITE_VFS_BUFFERS
                || p->iBufferOfst + p->nBuffer != currentPosition) {
                int rc = e1FlushBuffer(p);

                if (rc != SQLITE_OK) {
                    return (rc);
                }
            }

            p->iBufferOfst = currentPosition - p->nBuffer;

            bytesToWrite = SQLITE_VFS_BUFFERS - p->nBuffer;

            if (bytesToWrite > remainingBytes) {
                bytesToWrite = remainingBytes;
            }

            memcpy(&p->aBuffer[p->nBuffer], tempBuffer, bytesToWrite);
            p->nBuffer += bytesToWrite;

            remainingBytes -= bytesToWrite;
            currentPosition += bytesToWrite;
            tempBuffer += bytesToWrite;
        }
    } else {
        return (e1DirectWrite(p, buffer, size, offset));
    }

    return (SQLITE_OK);
}

static int e1Truncate(sqlite3_file *pFile, sqlite_int64 size)
{
    E1File *p = (E1File *)pFile;

    p->file.truncate(size);

    return (SQLITE_OK);
}

static int e1Sync(sqlite3_file *pFile, int flags)
{
    E1File *p = (E1File *)pFile;

    int rc = e1FlushBuffer(p);

    if (rc != SQLITE_OK) {
        return (rc);
    }

    p->file.flush();

    return (SQLITE_OK);
}

static int E1FileSize(sqlite3_file *pFile, sqlite_int64 *pSize)
{
    E1File *p = (E1File *)pFile;
    *pSize = p->file.size();

    return (SQLITE_OK);
}

static int
    e1Access(sqlite3_vfs *pVfs, const char *zPath, int flags, int *pResOut)
{
    *pResOut = (Hardware::sdcard.exists(zPath)) ? 1 : 0;

    return SQLITE_OK;
}

static int e1Open(sqlite3_vfs *pVfs,
                  const char *filename, // File to open, or 0 for a temp file
                  sqlite3_file *pFile,
                  int flags, // Input SQLITE_OPEN_XXX flags
                  int *pOutFlags // Output SQLITE_OPEN_XXX flags (or NULL)
)
{
    static const sqlite3_io_methods e1io = {
        1, // version
        e1Close, // close
        e1Read, // read
        e1Write, // write
        e1Truncate, // truncate
        e1Sync, // sync
        E1FileSize, // fileSize
        e1Lock, // lock
        e1Unlock, // unlock
        e1CheckReservedLock, // checkReservedLock
        E1FileControl, // fileControl
        e1SectorSize, // sectorSize
        e1DeviceCharacteristics // deviceCharacteristics
    };

    sqlite3_soft_heap_limit64(16384);
    sqlite3_hard_heap_limit64(32768);

    E1File *p = (E1File *)pFile;
    char *aBuf = 0;

    if (filename == nullptr) {
        return (SQLITE_IOERR);
    }

    if (flags & SQLITE_OPEN_MAIN_JOURNAL) {
        aBuf = (char *)sqlite3_malloc(SQLITE_VFS_BUFFERS);

        if (!aBuf) {
            return (SQLITE_NOMEM);
        }
    }

    //sqlite3_file base; // Base class. Must be first.
    //FileIoStream file;
    p->aBuffer = nullptr; // Pointer to malloc'd buffer
    p->nBuffer = 0; // Valid bytes of data in zBuffer
    p->iBufferOfst = 0; // Offset in file of zBuffer[0]

    if (flags & SQLITE_OPEN_CREATE || flags & SQLITE_OPEN_READWRITE
        || flags & SQLITE_OPEN_MAIN_JOURNAL) {
        if (!Hardware::sdcard.exists(filename)) {
            p->file = Hardware::sdcard.createOutputStream(
                filename, O_RDWR | O_CREAT | O_TRUNC);
        } else {
            p->file =
                Hardware::sdcard.createOutputStream(filename, O_RDWR | O_CREAT);
        }
    }

    if (!(p->file)) {
        if (aBuf) {
            sqlite3_free(aBuf);
        }

        return (SQLITE_CANTOPEN);
    }

    p->aBuffer = aBuf;

    if (pOutFlags) {
        *pOutFlags = flags;
    }

    p->base.pMethods = &e1io;

    return (SQLITE_OK);
}

static int e1Delete(sqlite3_vfs *pVfs, const char *zPath, int dirSync)
{
    Hardware::sdcard.deleteFile(zPath);
    return (SQLITE_OK);
}

static int
    e1FullPathname(sqlite3_vfs *pVfs, // VFS
                   const char *zPath, // Input path (possibly a relative path)
                   int nPathOut, // Size of output buffer in bytes
                   char *zPathOut // Pointer to output buffer
    )
{
    strncpy(zPathOut, zPath, nPathOut);
    zPathOut[nPathOut - 1] = '\0';

    return (SQLITE_OK);
}

static void *e1DlOpen(sqlite3_vfs *pVfs, const char *zPath)
{
    return (0);
}

static void e1DlError(sqlite3_vfs *pVfs, int nByte, char *zErrMsg)
{
    sqlite3_snprintf(nByte, zErrMsg, "Loadable extensions are not supported");
    zErrMsg[nByte - 1] = '\0';
}

static void (*e1DlSym(sqlite3_vfs *pVfs, void *pH, const char *z))(void)
{
    return (0);
}

static void e1DlClose(sqlite3_vfs *pVfs, void *pHandle)
{
    return;
}

static int e1Randomness(sqlite3_vfs *pVfs, int numBytes, char *randomNumber)
{
    for (int i = 0; i < numBytes; i++) {
        randomNumber[i] = random(0, 255);
    }

    return (SQLITE_OK);
}

static int e1Sleep(sqlite3_vfs *pVfs, int microseconds)
{
    delayMicroseconds(microseconds);

    return (microseconds);
}

static int e1CurrentTime(sqlite3_vfs *pVfs, double *pTime)
{
    *pTime = millis();

    return (SQLITE_OK);
}

sqlite3_vfs *sqlite3_e1vfs(void)
{
    static sqlite3_vfs e1vfs = {
        1, // version
        sizeof(E1File), // size of OS File
        MAXPATHNAME, // max pathname
        0, // pointer next
        "e1", // name of the VFS driver
        0, // pointer AppData
        e1Open, // open
        e1Delete, // delete
        e1Access, // access
        e1FullPathname, // fullPathname
        e1DlOpen, // dlOpen
        e1DlError, // dlError
        e1DlSym, // dlSym
        e1DlClose, // dlClose
        e1Randomness, // dandomness
        e1Sleep, // sleep
        e1CurrentTime, // xurrentTime
    };

    return (&e1vfs);
}

// as an example for future work
int registerFunctions(sqlite3 *db,
                      const char **pzErrMsg,
                      const struct sqlite3_api_routines *pThunk)
{
    //sqlite3_create_function(db, "examplefunc", 1,
    // SQLITE_UTF8 | SQLITE_DETERMINISTIC, 0, examplefunc, 0, 0);

    return (SQLITE_OK);
}

void errorLogCallback(void *pArg, int errCode, const char *message)
{
    logMessage("e1VFS error: errcode=%d, msg=%s", errCode, message);
}

int sqlite3_os_init(void)
{
    sqlite3_vfs_register(sqlite3_e1vfs(), 1);
    sqlite3_auto_extension((void (*)())registerFunctions);
    return (SQLITE_OK);
}

int sqlite3_os_end(void)
{
    return (SQLITE_OK);
}
