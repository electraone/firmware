#pragma once

#include "Component.h"
#include "MidiInputCallback.h"

class Slider final : public Component, private MidiInputCallback
{
public:
    Slider() : value(0), faderPosition(0), dimmed(false)
    {
        setName("slider");
    }

    // Slider lambdas
    std::function<void(int16_t value)> onChange;
    std::function<void(int16_t value)> onCompleted;

    void handleIncomingMidiMessage(const MidiInput &midiInput,
                                   const MidiMessage &midiMessage)
    {
        if ((midiMessage.getType() == MidiMessage::Type::ControlChange)
            && (midiMessage.getData1() == 1)) {
            setValue(midiMessage.getData2());
        }
    }

    void setValue(int16_t newValue)
    {
        value = newValue;
        faderPosition = map(newValue, min, max, 0, getWidth() - indicatorSize);
        repaint();
    }

    void onTouchMove(const TouchEvent &touchEvent) override
    {
        float stepDetail = getWidth() / (float)(max);
        int16_t newValue =
            constrain(ceil(touchEvent.getX() / stepDetail), min, max);
        setValue(newValue);

        if (onChange) {
            onChange(value);
        }
    }

    void onTouchUp(const TouchEvent &touchEvent) override
    {
        if (onCompleted) {
            onCompleted(value);
        }
    }

    void setDimmed(bool shouldBeDimmed)
    {
        dimmed = shouldBeDimmed;
    }

    bool isDimmed(void) const
    {
        return (dimmed);
    }

    void paint(Graphics &g) override
    {
        uint16_t faderOffsetY = indicatorSize / 2;

        // Clear the space of control's bounds
        g.fillAll(ElectraColours::rgb565NumericBlack);

        // Set colour for the track
        g.setColour(ElectraColours::rgb565NumericFrame);

        // Slider track
        g.drawRect(0,
                   faderOffsetY - 1,
                   getWidth(),
                   2);

        // Slider indicator
        g.copyFromToolkit(540,
                          436,
                          faderPosition,
                          faderOffsetY - indicatorSize / 2,
                          indicatorSize,
                          indicatorSize);

        // Set colour for the track
        g.setColour(ElectraColours::rgb565NumericWhite);

        g.fillElipse(faderPosition + indicatorSize / 2,
                     faderOffsetY,
                     8,
                     8);

        // Display value
        char stringValue[5];
        itoa(value, stringValue, 10);
        g.printText(0,
                    getHeight() - 20,
                    stringValue,
                    TextStyle::mediumTransparent,
                    getWidth(),
                    TextAlign::center);

        // Apply a dim
        if (dimmed == true) {
            g.dim(0,
                  0,
                  getWidth(),
                  getHeight(),
                  ElectraColours::rgb565NumericBlack);
        }
    }

private:
    static const int16_t min = 0;
    static const int16_t max = 127;
    static const uint8_t indicatorSize = 22;

    int16_t value;
    int16_t faderPosition;
    bool dimmed;
};
