#pragma once

#include "Hardware.h"
#include "System.h"
#include "LocalFile.h"
#include "WindowManager.h"
#include "StatusBar.h"
#include "FlashMessage.h"
#include "luabridge.h"

#define CREATE_ELECTRA_APPLICATION(AppClass)                                   \
    void createApplication(void);                                              \
    void createApplication(void)                                               \
    {                                                                          \
        App::set(new AppClass());                                              \
    }

#define START_ELECTRA_APPLICATION(AppClass) CREATE_ELECTRA_APPLICATION(AppClass)

class App
{
protected:
    App();
    static App *appInstance;

public:
    static void set(App *newAppInstance);
    static App *get(void);

    App(App &other) = delete;
    void operator=(const App &) = delete;
    virtual ~App() = default;

    virtual void initialise(void);
    virtual const char *getApplicationName(void) const;
    virtual const char *getApplicationVersion(void) const;
    virtual const char *getApplicationSandbox(void) const;
    virtual Window *getMainWindow(void);

    // \todo these will be moved elsewhere
    virtual void handleIncomingControlMessage(MidiInput &midiInput,
                                              MidiMessage &midiMessage);
    virtual void handleElectraSysex(uint8_t port, const SysexBlock &sysexBlock);
    virtual void execute(const char *filename);
    virtual bool handleCtrlFileReceived(LocalFile file,
                                        ElectraCommand::Object fileType);
    virtual bool handleCtrlFileRemoved(uint8_t bankNumber,
                                       uint8_t slot,
                                       ElectraCommand::Object fileType);
    virtual void runUserTask(void);

    void clearScreen(void);
    void paintScreen(void);
    void indicateMidiActivity(uint8_t port,
                              Direction direction,
                              MidiMessage::Type msgType);
    virtual uint8_t getUsbHostPortAssigment(const char *productName);

    // These are kept public on purpose
public:
    bool enableMidi; // \todo enableMidi needs to be renamed and moved to System
    StatusBar statusBar;
    FlashMessage flashMessage;
    MemoryPool sysexPool;
};
