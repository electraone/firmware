#include "System.h"
#include "helpers.h"

/* This is to be able to initiate USB stack in here. To give more
 * control over the USB device configuration
 */
extern "C" void usb_init(uint8_t keyboardEnabled,
                         uint8_t mouseEnabled,
                         uint8_t keymediaEnabled);

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
lua_State *System::L = nullptr;
