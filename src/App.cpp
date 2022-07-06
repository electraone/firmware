#include "App.h"

App *App::appInstance = nullptr;

App *App::get(void)
{
    return appInstance;
}

void App::set(App *newAppInstance)
{
    appInstance = newAppInstance;
}

App::App()
    : sysexPool(&Hardware::screen, SDRAM_PAGE_0, SDRAM_PAGE_SIZE),
      flashMessage(Rectangle(0, 570, 1024, 29),
                   "",
                   TextStyle::mediumWhiteOnBlack,
                   TextAlign::center),
      enableMidi(false)
{
}

void App::initialise(void)
{
}

const char *App::getApplicationName(void) const
{
    return ("default");
}

const char *App::getApplicationVersion(void) const
{
    return ("1.0.0");
}

const char *App::getApplicationSandbox(void) const
{
    return ("default");
}

Window *App::getMainWindow(void)
{
    return (nullptr);
}

// --------------------------------------------------------------------------

#include <CircularBuffer.h>

// \todo To be removed.
void App::handleIncomingControlMessage(MidiInput &midiInput,
                                       MidiMessage &midiMessage)
{
}

void App::handleElectraSysex(uint8_t port, const SysexBlock &sysexBlock)
{
}

void App::execute(const char *filename)
{
}

bool App::handleCtrlFileReceived(uint8_t port,
                                 LocalFile file,
                                 ElectraCommand::Object fileType)
{
    return (true);
}

bool App::handleCtrlFileRemoved(uint8_t bankNumber,
                                uint8_t slot,
                                ElectraCommand::Object fileType)
{
    return (true);
}

void App::runUserTask(void)
{
}

void App::indicateMidiActivity(uint8_t port,
                               Direction direction,
                               MidiMessage::Type msgType)
{
    statusBar.indicate(MidiInterface::Type::MidiIo, port, direction, msgType);
    statusBar.indicate(
        MidiInterface::Type::MidiUsbDev, port, direction, msgType);
    if (USBDevices[port].midiDevice != NULL) {
        statusBar.indicate(
            MidiInterface::Type::MidiUsbHost, port, direction, msgType);
    }
}

uint8_t App::getUsbHostPortAssigment(const char *productName)
{
    return (0);
}

void App::clearScreen(void)
{
    //flushRepaintGraphics();
    //Hardware::screen.clear();
    statusBar.forceRepaint();
}

void App::paintScreen(void)
{
    System::windowManager.repaintActive();
}
