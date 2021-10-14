#pragma once

#include "Ui.h"
#include "Drawings.h"
#include "TextGraphics.h"
#include "MidiOutput.h"

class MainComponent : public Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(Graphics &g) override;
    void resized(void) override;

    void onPotChange(const PotEvent &potEvent) override;
    void onPotTouchDown(const PotEvent &potEvent) override;
    void onPotTouchUp(const PotEvent &potEvent) override;

private:
    static constexpr uint8_t numPads = 8;
    static constexpr uint8_t numKnobs = 6;
    static constexpr uint8_t numBars = 6;

    Pad *pad[8];
    Drawings *drawings;
    Knob *knob[numKnobs];
    BarVertical *bar[numBars];
    MidiOutput usbDevOutput;
};
