#include "Hardware.h"
#include "System.h"
#include "App.h"
#include "SplashScreen.h"
#include "MidiOutput.h"

/*
 * Prototype of the application creation function.
 * Required by START_ELECTRA_APPLICATION() macro.
 */
extern App *createApplication(void);

void initialise(void)
{
    createApplication();

    // Initialise hardware components
    Hardware::initialise();

    // Initilise the base system
    System::initialise();

    // Enable Logger
    System::logger.enable();

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
        System::logger.write(
            LOG_ERROR, "UI asset file not found: file=%s", UI_FILE);
    } else {
        Hardware::screen.loadEssentialFont();
        displaySplash(Hardware::screen,
                      App::get()->getApplicationName(),
                      App::get()->getApplicationVersion(),
                      System::runtimeInfo.getElectraInfoHwRevision());
        Hardware::screen.initUIToolkit();
    }
    System::logger.write(LOG_INFO, "UI assets: Loaded and initialised");

    // Initialize MIDI interfaces
    MidiInterface::get(MidiInterface::Type::MidiIo)->initialise();
    MidiInterface::get(MidiInterface::Type::MidiUsbDev)->initialise();
    MidiInterface::get(MidiInterface::Type::MidiUsbHost)->initialise();
    System::logger.write(LOG_INFO, "MIDI interfaces: initialised");

    // Clear the repaint queue \todo does not beliong here
    System::tasks.clearRepaintGraphics();

    // Initialise the user application (App)
    App::get()->initialise();

    // Clear and repaint the application screen.
    App::get()->clearScreen();
    App::get()->paintScreen();

    // Enable standard system tasks
    System::tasks.enableAll();
    System::logger.write(LOG_INFO,
                         "kernel task manager: enable software tasks");

    // Enable MIDI
    System::tasks.enableMidi();
    System::logger.write(LOG_INFO, "setup: enable readMidi task");

    // Get saved status of the logger (as saved in the RuntimeInfo)
    System::logger.setStatus(System::runtimeInfo.getLoggerStatus());

    App::get()->enableMidi = true;
    MidiOutput::sendAvailable(MidiInterface::Type::MidiUsbDev, 2);
}

void processEvents(void)
{
    System::tasks.execute();
}
