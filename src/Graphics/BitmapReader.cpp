#include "BitmapReader.h"
#include "Colours.h"
#include "System.h"
#include "Hardware.h"
#include "settings.h"

BitmapReader::BitmapReader(const uint8_t CSp, const uint8_t RSTp)
    : FrameBuffer(CSp, RSTp)
{
}

BitmapReader::~BitmapReader()
{
}

void BitmapReader::loadBMP(const char *filename,
                           uint16_t lines,
                           bool progressBar)
{
    bmpHeader header;
    bmpInfoHeader infoheader;
    FileIoStream file;

    if ((file = Hardware::sdcard.createInputStream(filename)) == 0) {
        System::logger.write(ERROR, "Unable to open BMP file: %s", filename);
        return;
    }

    // Read the file header
    readUShort(file, &header.type, false);
    readUInt(file, &header.size, false);
    readUShort(file, &header.reserved1, false);
    readUShort(file, &header.reserved2, false);
    readUInt(file, &header.offset, false);

    // Read the BMP info header
    if (file.read(&infoheader, sizeof(bmpInfoHeader))
        != sizeof(bmpInfoHeader)) {
        System::logger.write(ERROR, "Failed to read BMP info header");
        return;
    }

    // Disable interrupts to prevent collision in writting to SDRAM
    noInterrupts();

    if (infoheader.height < 0) {
        infoheader.height = infoheader.height * -1;
    }

    // Seek to the start of the image data
    file.seek(header.offset);

    uint16_t buffer[1024];

    setCanvasAddress(FRAME_UI_TOOLKIT);
    clearScreen(Colours565::black);

    // raw background for the progress bar
    if (progressBar == true) {
        setCanvasAddress(FRAME_BUFFER_2);
        setForegroundColor(0x4208);
        fillRect(407, 330, 200, 6);
    }

    setAddress(0, 0);

    for (uint16_t y = 0; y < lines; y++) {
        if (file.read(&buffer, sizeof(buffer)) != sizeof(buffer)) {
            System::logger.write(ERROR, "Image read failed");
            return;
        }

        /*
		 * draw progress bar
		 */
        if (progressBar == true) {
            if ((y % 3) == 0) {
                setCanvasAddress(FRAME_BUFFER_2);
                setForegroundColor(Colours565::white);
                fillRect(407 + (y / 3), 330, 1, 6);

                setForegroundColor(Colours565::black);
                fillRect(407, 330, 1, 1);
                fillRect(407, 335, 1, 1);
                fillRect(606, 330, 1, 1);
                fillRect(606, 335, 1, 1);
            }
        }

        setCanvasAddress(FRAME_UI_TOOLKIT);

        for (uint16_t x = 0; x < 1024; x++) {
            writeRAM16(buffer[x]);
        }
    }

    // Enable interrupts again
    interrupts();

    file.close();
}

bool readUShort(FileIoStream &file, short unsigned *n, int swap)
{
    if (file.read(n, 2) != 2) {
        return (false);
    }
    return (true);
}

bool readUInt(FileIoStream &file, unsigned int *n, int swap)
{
    if (file.read(n, 4) != 4) {
        return (false);
    }
    return (true);
}

void BitmapReader::initUIToolkit(void)
{
    loadBMP(UI_FILE, 600);
}

void BitmapReader::loadEssentialFont(void)
{
    loadBMP(UI_FILE, 73, false);
}
