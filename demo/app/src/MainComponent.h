#pragma once

#include "Component.h"
#include "Slider.h"
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
    Slider *slider;
    Drawings *drawings;
    MidiOutput usbDevOutput;
};
