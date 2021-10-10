#pragma once

#include "Component.h"
#include "Rect.h"
#include "RectRound.h"
#include "Triangle.h"
#include "Circle.h"
#include "ElectraImage.h"
#include "Indicator.h"
#include "MidiInputCallback.h"

class Drawings final : public Component, private MidiInputCallback
{
public:
    Drawings() : value(0)
    {
        setName("drawings");
        rectEmpty = new Rect(false);
        rectFilled = new Rect(true);
        rectRoundEmpty = new RectRound(false);
        rectRoundFilled = new RectRound(true);
        triangleEmpty = new Triangle(false);
        triangleFilled = new Triangle(true);
        circleEmpty = new Circle(false);
        circleFilled = new Circle(true);
        electraImage = new ElectraImage();
        indicator = new Indicator();

        addAndMakeVisible(rectEmpty);
        addAndMakeVisible(rectFilled);
        addAndMakeVisible(rectRoundEmpty);
        addAndMakeVisible(rectRoundFilled);
        addAndMakeVisible(triangleEmpty);
        addAndMakeVisible(triangleFilled);
        addAndMakeVisible(circleEmpty);
        addAndMakeVisible(circleFilled);
        addAndMakeVisible(electraImage);
        addAndMakeVisible(indicator);
    }

    void resized(void) override
    {
        rectEmpty->setBounds(0, padding, size, size);
        rectFilled->setBounds(spacing, padding, size, size);
        rectRoundEmpty->setBounds(spacing * 2, padding, size, size);
        rectRoundFilled->setBounds(spacing * 3, padding, size, size);
        triangleEmpty->setBounds(spacing * 4, padding, size, size);
        triangleFilled->setBounds(
            spacing * 5 - spacing / 2 + padding, padding, size, size);
        circleEmpty->setBounds(spacing * 6 - spacing / 2, padding, size, size);
        circleFilled->setBounds(spacing * 7 - spacing / 2, padding, size, size);
        electraImage->setBounds(
            spacing * 8 - spacing / 2 + padding, padding, 94, 80);
        indicator->setBounds(0, 0, spacing, getHeight());
    }

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
        int16_t indicatorPositionX =
            map(newValue, min, max, 0, getWidth() - indicatorWidth);
        logMessage("indicator: %d (min=%d, max=%d value=%d)",
                   indicatorPositionX,
                   min,
                   max,
                   newValue);
        value = newValue;
        indicator->setX(indicatorPositionX);
        repaint();
    }

    void paint(Graphics &g) override
    {
        uint16_t width = getWidth();

        // Clear the component area
        g.fillAll(ElectraColours::rgb565NumericBlack);

        g.setColour(ElectraColours::rgb565NumericWhite);

        // Line
        g.drawRect(0, 0, width, 1);

        // Draw pixel
        for (uint16_t x = 0; x < width; x += 4) {
            g.drawPixel(x, spacing + padding);
        }
    }

    void onTouchDown(const TouchEvent &touchEvent) override
    {
        logMessage("callback: drawings");
    }

private:
    static const uint8_t spacing = 90;
    static const uint8_t padding = 10;
    static const uint8_t size = spacing - padding;
    static const int16_t min = -64;
    static const int16_t max = 63;
    static const int16_t indicatorWidth = spacing;

    int16_t value;

    Rect *rectEmpty;
    Rect *rectFilled;
    RectRound *rectRoundEmpty;
    RectRound *rectRoundFilled;
    Triangle *triangleEmpty;
    Triangle *triangleFilled;
    Circle *circleEmpty;
    Circle *circleFilled;
    ElectraImage *electraImage;
    Indicator *indicator;
};
