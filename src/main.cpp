#include "Hardware.h"
#include "System.h"
#include "App.h"
#include "SplashScreen.h"

/*
 * Prototype of the application creation function.
 * Required by START_ELECTRA_APPLICATION() macro.
 */
extern App *createApplication(void);

void initialise(void)
{
    createApplication();

    // \todo Lua will go to its own class
    L = nullptr;

    // \todo This will go to the Logger class
    loggerEnabled = true;

    // Initialise hardware components
    Hardware::initialise();

    // Initilise the base system
    System::initialise();

    // Create the sandbox dir. \todo sandboxing needs to be improved
    Hardware::sdcard.createDirectory(App::get()->getApplicationSandbox());

    // Set the brightness of the LCD \todo belongs to hardware
    uint16_t electraInfoBrightness =
        System::runtimeInfo.getElectraInfoBrightness();

    if (electraInfoBrightness != 0) {
        Hardware::screen.setBacklightbrightness(electraInfoBrightness);
    }

    // Load the UI toolkit
    if (Hardware::sdcard.exists(UI_FILE) == false) {
        logMessage("UI asset file not found: file=%s", UI_FILE);
    } else {
        Hardware::screen.loadEssentialFont();
        displaySplash(Hardware::screen,
                      App::get()->getApplicationName(),
                      App::get()->getApplicationVersion(),
                      System::runtimeInfo.getElectraInfoHwRevision());
        Hardware::screen.initUIToolkit();
    }
    logMessage("UI assets: Loaded and initialised");

    // Initialize MIDI interfaces
    MidiInterface::get(MidiInterface::Type::MidiIo)->initialise();
    MidiInterface::get(MidiInterface::Type::MidiUsbDev)->initialise();
    MidiInterface::get(MidiInterface::Type::MidiUsbHost)->initialise();
    logMessage("MIDI interfaces: initialised");

    // Clear the repaint queue \todo does not beliong here
    System::tasks.clearRepaintGraphics();

    // Initialise the user application (App)
    App::get()->initialise();

    // Clear and repaint the application screen.
    App::get()->clearScreen();
    //App::get()->paintScreen();

    // Enable standard system tasks
    System::tasks.enableAll();
    logMessage("kernel task manager: enable software tasks");

    // Enable MIDI
    System::tasks.enableMidi();
    logMessage("setup: enable readMidi task");

    // Get status of the logger \todo move to Logger class
    loggerEnabled = System::runtimeInfo.getLoggerStatus();
}

void processEvents(void)
{
    System::tasks.execute();
}
