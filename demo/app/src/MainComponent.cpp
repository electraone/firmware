#include "MainComponent.h"
#include "helpers.h"

MainComponent::MainComponent()
    : usbDevOutput(MidiInterface::Type::MidiUsbDev, 0)
{
    // Set the component name
    setName("mainComponent");

    // Add an example of graphics object
    drawings = new Drawings;
    addAndMakeVisible(drawings);

    // Knobs
    for (uint8_t i = 0; i < numKnobs; i++) {
        knob[i] = new Knob();
        knob[i]->setColour(Colours::olive);
        knob[i]->assignPot(i);
        addAndMakeVisible(knob[i]);
    }

    // Set up a slider (an example of a component)
    for (uint8_t i = 0; i < numBars; i++) {
        bar[i] = new BarVertical;
        bar[i]->setColour(Colours::peru);
        bar[i]->assignPot(i + numKnobs);

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

    assignAllPots();

    // \todo this does not work. leaving it out for now.
    //setBounds(getParentComponent()->getLocalBounds());

    setBounds(0, 0, 1024, 560);
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint(Graphics &g)
{
    g.fillAll(ElectraColours::rgb565NumericBlack);
    g.printText(0,
                40,
                "Demo Application",
                TextStyle::largeTransparent,
                getWidth(),
                TextAlign::center);
}

void MainComponent::resized()
{
    drawings->setBounds(122, 100, 780, 160);

    for (uint8_t i = 0; i < numKnobs; i++) {
        knob[i]->setBounds(35 + (170 * i), 285, 100, 100);
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
