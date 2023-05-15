#include "App.h"
#include "MainComponent.h"
#include "MidiInputCallback.h"
#include "ButtonListener.h"
#include "Model.h"

class DemoApp : public App, private MidiInputCallback, private ButtonListener
{
public:
    DemoApp()
    {
        assignAllButtons();
    }

    const char *getApplicationName(void) const override
    {
        return ("Demo");
    }

    const char *getApplicationVersion(void) const override
    {
        return ("1.0.0");
    }

    const char *getApplicationSandbox(void) const override
    {
        return ("demo");
    }

    void initialise(void) override
    {
        System::logger.write(
            LOG_ERROR, "sqlite version: %s\n", sqlite3_libversion());

        model.attach("synth.db");
        model.create();

        if (model.getCount() == 0) {
            model.insertRows();
        }

        model.query();
        model.close();

        System::logger.write(LOG_ERROR, "setup completed");
        mainWindow.repaint();
    }

    class MainWindow : public Window
    {
    public:
        MainWindow()
        {
            setOwnedContent(new MainComponent());
            setVisible(true);
        }
    };

    void onButtonDown(uint8_t buttonId) override
    {
        System::logger.write(LOG_ERROR, "buttonDown: buttonId=%d", buttonId);
    }

    void onButtonLongHold(uint8_t buttonId) override
    {
        System::logger.write(
            LOG_ERROR, "buttonLongHold: buttonId=%d", buttonId);
    }

    void onButtonUp(uint8_t buttonId) override
    {
        System::logger.write(LOG_ERROR, "buttonUp: buttonId=%d", buttonId);
    }

    void handleIncomingMidiMessage(const MidiInput &midiInput,
                                   const MidiMessage &midiMessage) override
    {
        if (midiMessage.isSysEx()) {
            System::logger.write(
                LOG_ERROR,
                "---< sysex start: interface=%s, port=%d >---",
                MidiInterface::getName(midiInput.getInterfaceType()),
                midiInput.getPort());

            SysexBlock sysexBlock = midiMessage.getSysExBlock();
            size_t sysexLength = sysexBlock.getLength();

            for (size_t i = 0; i < sysexLength; i++) {
                byte sysexByte = sysexBlock.peek(i);
                System::logger.write(
                    LOG_ERROR, "%d> %X (%c)", i, sysexByte, sysexByte);
            }

            System::logger.write(LOG_ERROR, "---------------------");
        }
    }

private:
    MainWindow mainWindow;
    Model model;
};

// This macro instructs main() routine to launch the app.
START_ELECTRA_APPLICATION(DemoApp)
