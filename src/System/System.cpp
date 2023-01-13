#include "System.h"
#include "helpers.h"

void System::initialise(void)
{
    // Initialize Random number generator
    randomSeed(analogRead(0));

    // initialize non-volatile storage if not set
    runtimeInfo.read();

    // initialize USB device.

    // First time configuration
    // This is to bring older Electras to known state of the USB device
    // configuration.
    if (runtimeInfo.getUsbDevStatus() != 1) {
        runtimeInfo.setUsbDevStatus(1);
        runtimeInfo.setKeyboardStatus(0);
        runtimeInfo.setMouseStatus(0);
        runtimeInfo.setKeymediaStatus(0);
    }

    // Configure the USB device interfaces
    uint8_t keyboardEnabled = 0;
    uint8_t mouseEnabled = 0;
    uint8_t keymediaEnabled = 0;

    if (runtimeInfo.getUsbDevStatus() == 1) {
        keyboardEnabled = runtimeInfo.getKeyboardStatus();
        mouseEnabled = runtimeInfo.getMouseStatus();
        keymediaEnabled = runtimeInfo.getKeymediaStatus();
    }

    usb_init(keyboardEnabled, mouseEnabled, keymediaEnabled);
    logMessage("USB device ports: initialised");
}

SystemTasks System::tasks;
WindowManager System::windowManager;
Context System::context;
RuntimeInfo System::runtimeInfo;
Logger System::logger;
lua_State *System::L = nullptr;

bool System::repaintActive = false;
