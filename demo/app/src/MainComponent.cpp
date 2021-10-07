#include "MainComponent.h"
#include "helpers.h"

MainComponent::MainComponent()
    : usbDevOutput(MidiInterface::Type::MidiUsbDev, 0)
{
    // Set the component name
    setName("mainComponent");

    // Set up a slider (an example of a component)
    slider = new SliderHorizontal;
    slider->setValue(0);

    slider->onValueChange = [this](int16_t value) {
        logMessage("value = %d", value);
        MidiMessage cc = MidiMessage::controllerEvent(1, 1, value);
        usbDevOutput.sendMessageNow(cc);
        drawings->setValue(value);
    };

    slider->onDragEnd = [this](int8_t value) {
        logMessage("Final value = %d", value);
    };

    addAndMakeVisible(slider);

    // Add an example of graphics object
    drawings = new Drawings;
    addAndMakeVisible(drawings);

    // Add a text object
    addChildComponent(new TextGraphics(Rectangle(0, 340, 1024, 20),
                                       "Mod Wheel Demo",
                                       TextStyle::largeWhiteOnBlack,
                                       TextAlign::center));

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
                500,
                "This is a text in the window",
                TextStyle::largeTransparent,
                getWidth(),
                TextAlign::center);
}

void MainComponent::resized()
{
    drawings->setBounds(122, 100, 780, 160);
    slider->setBounds(337, 400, 350, 60);
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
