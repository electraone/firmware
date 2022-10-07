#include "MainComponent.h"
#include "helpers.h"

MainComponent::MainComponent()
    : usbDevOutput(MidiInterface::Type::MidiUsbDev, 0)
{
    // Set the component name
    setName("mainComponent");

    // Add an example of graphics object

    // Pads
    for (uint8_t i = 0; i < numPads; i++) {
        pad[i] = new Pad;
        pad[i]->setColour(((i % 4) == 0) ? Colours::yellow : Colours::red);
        pad[i]->setState(((i % 3) == 0) ? true : false);
        addAndMakeVisible(pad[i]);
    }

    // Custom component
    drawings = new Drawings;
    addAndMakeVisible(drawings);

    // Knobs
    for (uint8_t i = 0; i < numKnobs; i++) {
        knob[i] = new Knob();
        knob[i]->setColour(Colours::darkslateblue);
        knob[i]->setRange(-64.0f, 63.0f);
        knob[i]->setValue(0.0f);
        knob[i]->assignPot(i);
        addAndMakeVisible(knob[i]);
    }

    // Set up a slider (an example of a component)
    for (uint8_t i = 0; i < numBars; i++) {
        bar[i] = new BarVertical;
        bar[i]->setColour(Colours::peru);
        bar[i]->setRange(-64.0f, 63.0f);
        bar[i]->setValue(0.0f);
        bar[i]->assignPot(i);
        bar[i]->assignPot(i + numKnobs);

        /*
        bar[i]->onValueChange = [this](int16_t value) {
            logMessage("value = %d", value);
            MidiMessage cc = MidiMessage::controllerEvent(1, 1, value);
            usbDevOutput.sendMessageNow(cc);
            drawings->setValue(value);
        };

        bar[i]->onDragEnd = [this](int8_t value) {
            logMessage("Final value = %d", value);
        };

        addAndMakeVisible(bar[i]);
    }
*/
        assignAllPots();

        // \todo this does not work. leaving it out for now.
        //setBounds(getParentComponent()->getLocalBounds());

        setBounds(0, 0, 1024, 560);
    }

    MainComponent::~MainComponent()
    {
    }

    void MainComponent::paint(Graphics & g)
    {
        g.fillAll(Colours::black);
        return;
        g.printText(0,
                    40,
                    "Demo Application",
                    TextStyle::largeTransparent,
                    getWidth(),
                    TextAlign::center);
    }

    void MainComponent::resized()
    {
        for (uint8_t i = 0; i < numPads; i++) {
            pad[i]->setBounds(48 + (122 * i), 40, 70, 70);
        }

        drawings->setBounds(122, 150, 780, 140);

        for (uint8_t i = 0; i < numKnobs; i++) {
            knob[i]->setBounds(35 + (170 * i), 300, 100, 100);
        }

        for (uint8_t i = 0; i < numBars; i++) {
            bar[i]->setBounds(50 + (170 * i), 430, 70, 120);
        }
    }

    void MainComponent::onPotChange(const PotEvent &potEvent)
    {
        logMessage("potChange: potId=%d, relativeChange=%d",
                   potEvent.getPotId(),
                   potEvent.getRelativeChange());
    }

    void MainComponent::onPotTouchDown(const PotEvent &potEvent)
    {
        logMessage("potTouchDown: potId=%d", potEvent.getPotId());
    }

    void MainComponent::onPotTouchUp(const PotEvent &potEvent)
    {
        logMessage("potTouchUp: potId=%d", potEvent.getPotId());
    }
