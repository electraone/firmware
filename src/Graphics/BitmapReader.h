#pragma once

#include "FrameBuffer.h"
#include "PersistentStorage.h"

/*
 * BMP parsing structures
 */
typedef struct {
    unsigned short int type; // Magic identifier
    unsigned int size; // File size in bytes
    unsigned short int reserved1, reserved2;
    unsigned int offset; // Offset to image data, bytes
} bmpHeader;

typedef struct {
    unsigned int size; // Header size in bytes
    int width, height; // Width and height of image
    unsigned short int planes; // Number of colour planes
    unsigned short int bits; // Bits per pixel
    unsigned int compression; // Compression type
    unsigned int imagesize; // Image size in bytes
    int xresolution, yresolution; // Pixels per meter
    unsigned int ncolours; // Number of colours
    unsigned int importantcolours; // Important colours
} bmpInfoHeader;

bool readUShort(FileIoStream &, unsigned short *, int);
bool readUInt(FileIoStream &, unsigned int *, int);

/** \brief a representation of the Screen - the object describing what user
 *         sees on the LCD display
 */
class BitmapReader : public virtual FrameBuffer
{
public:
    BitmapReader(const uint8_t CSp, const uint8_t RSTp);
    ~BitmapReader();

    void loadBMP(const char *filename,
                 uint16_t lines = 600,
                 bool progressBar = true);

    void initUIToolkit(void);
    void loadEssentialFont(void);
};
