#include "Knob.h"

Knob::Knob()
{
}

void Knob::onTouchMove(const TouchEvent &touchEvent)
{
    float step = getHeight() / (float)(max - min);
    int16_t newValue =
        constrain((int16_t)ceil((getHeight() - touchEvent.getY()) / step + min),
                  min,
                  max);

    setValue(newValue);

    if (onValueChange) {
        onValueChange(value);
    }
}

void Knob::onTouchDown(const TouchEvent &touchEvent)
{
}

void Knob::onTouchUp(const TouchEvent &touchEvent)
{
}

void Knob::paint(Graphics &g)
{
    uint32_t colourTrack = Colours::changeBrightness(SCREEN_BPP, colour, 0.3f);
    uint32_t colourCenter = Colours::changeBrightness(SCREEN_BPP, colour, 0.2f);

    float angle = map((float)value, min, max, angleTrackStart, angleTrackEnd);
    float angleZero =
        map((float)0.0f, min, max, angleTrackStart, angleTrackEnd);

    uint16_t xCentre = getWidth() / 2;
    uint16_t yCentre = getWidth() / 2;
    uint16_t radius = getWidth() / 2 - 1;

    // Track start and end points
    uint16_t xTrackStart = cosf(angleTrackStart) * (float)radius + xCentre;
    uint16_t yTrackStart = sinf(angleTrackStart) * (float)radius + xCentre;
    uint16_t xTrackEnd = cosf(angleTrackEnd) * (float)radius + xCentre;
    uint16_t yTrackEnd = sinf(angleTrackEnd) * (float)radius + xCentre;

    // Clear the component area
    g.fillAll(Colours::black);

    //Paint the track
    g.setColour(colourTrack);
    g.fillCircle(xCentre, yCentre, radius);

    // Paint and apply active fader
    paintValue(g, xCentre, yCentre, radius, angleZero, angle);

    // Paint the outline
    g.setColour(colour);
    g.drawCircle(xCentre, yCentre, radius);

    // Mask the track
    g.setColour(Colours::black);
    g.fillTriangle(
        xCentre, yCentre, xTrackStart, yTrackStart, xTrackEnd, yTrackEnd);
    g.fillRect(xTrackStart,
               yTrackStart,
               xTrackEnd - xTrackStart,
               getHeight() - yTrackStart + 1);

    // Mask the centre
    g.setColour(Colours::black);
    g.fillCircle(xCentre, yCentre, getWidth() / 3 - 1);
    g.setColour(colourCenter);
    g.fillCircle(xCentre, yCentre, getWidth() / 4 - 1);
}

void Knob::paintValue(Graphics &g,
                      uint16_t xCentre,
                      uint16_t yCentre,
                      uint16_t radius,
                      float angleZero,
                      float angle)
{
    uint32_t address = g.getCanvasAddress();
    g.setCanvasAddress(FRAME_MASK);
    g.setForegroundColor(Colours::black);

    if (angleZero < angle) {
        drawCurve(g, xCentre, yCentre, radius, angleZero, angle);
    } else {
        drawCurve(g, xCentre, yCentre, radius, angle, angleZero);
    }

    g.copy(FRAME_MASK,
           getScreenX(),
           getScreenY(),
           address,
           0,
           0,
           getWidth(),
           getHeight());
    g.setCanvasAddress(address);
}

void Knob::drawCurve(Graphics &g,
                     uint16_t xCentre,
                     uint16_t yCentre,
                     uint16_t radius,
                     float angleStart,
                     float angleEnd)
{
    uint16_t width = getWidth();
    uint16_t height = getHeight();

    // Translate angles to coordinates
    uint16_t xStart = cosf(angleStart) * (float)radius + xCentre;
    uint16_t yStart = sinf(angleStart) * (float)radius + xCentre;
    uint16_t xEnd = cosf(angleEnd) * (float)radius + xCentre;
    uint16_t yEnd = sinf(angleEnd) * (float)radius + xCentre;

    // Find the quadrands where start and end points are located
    uint8_t quadrantStart = getQuadrant(angleStart);
    uint8_t quadrantEnd = getQuadrant(angleEnd);

    // Clears curve area
    g.fillRect(0, 0, width, height);

    // Paint 360 degree curve
    g.setColour(colour);
    g.fillCircle(xCentre, yCentre, radius);

    // Hide unused segments
    for (uint8_t i = 0; i < 4; i++) {
        maskUnused(g, xCentre, yCentre, i, quadrantStart, quadrantEnd);
    }

    // Mask start and end part of the curve
    uint16_t x1 = 0;
    uint16_t y1 = 0;
    uint16_t x2 = 0;
    uint16_t y2 = 0;

    // Set the masking colour
    g.setColour(Colours::black);

    // Mask the curve segments
    if (quadrantEnd == 0) {
        x1 = 0;
        y1 = yCentre;
        x2 = 0;
        y2 = height - 1;
    } else if (quadrantEnd == 1) {
        x1 = xCentre;
        y1 = 0;
        x2 = 0;
        y2 = 0;
    } else if (quadrantEnd == 2) {
        x1 = width - 1;
        y1 = yCentre;
        x2 = width - 1;
        y2 = 0;
    } else if (quadrantEnd == 3) {
        x1 = yCentre;
        y1 = height - 1;
        x2 = width - 1;
        y2 = height - 1;
    }

    maskCurveSeqment(g, xCentre, yCentre, xEnd, yEnd, x1, y1, x2, y2);

    // Mask the curve segments
    if (quadrantStart == 0) {
        x1 = xCentre;
        y1 = height - 1;
        x2 = 0;
        y2 = height - 1;
    } else if (quadrantStart == 1) {
        x1 = 0;
        y1 = yCentre;
        x2 = 0;
        y2 = 0;
    } else if (quadrantStart == 2) {
        x1 = xCentre;
        y1 = 0;
        x2 = width - 1;
        y2 = 0;
    } else if (quadrantStart == 3) {
        x1 = width - 1;
        y1 = yCentre;
        x2 = width - 1;
        y2 = height - 1;
    }

    maskCurveSeqment(g, xCentre, yCentre, xStart, yStart, x1, y1, x2, y2);
}

void Knob::maskUnused(Graphics &g,
                      uint16_t xCentre,
                      uint16_t yCentre,
                      uint8_t quadrant,
                      uint8_t quadrantStart,
                      uint8_t quadrantEnd)
{
    uint16_t x1 = 0;
    uint16_t y1 = 0;
    uint16_t width = getWidth();
    uint16_t height = getHeight();

    if (!((quadrant >= quadrantStart) && (quadrant <= quadrantEnd))) {
        if (quadrant == 0) {
            x1 = -(width / 2);
            y1 = height / 2;
        } else if (quadrant == 1) {
            x1 = -(width / 2);
            y1 = -(height / 2);
        } else if (quadrant == 2) {
            x1 = width / 2;
            y1 = -(height / 2);
        } else if (quadrant == 3) {
            x1 = width / 2;
            y1 = height / 2;
        }

        g.setColour(Colours::black);
        g.fillRect(xCentre, yCentre, x1, y1);
    }
}

void Knob::maskCurveSeqment(Graphics &g,
                            uint16_t x1,
                            uint16_t y1,
                            uint16_t x2,
                            uint16_t y2,
                            uint16_t x3,
                            uint16_t y3,
                            uint16_t x4,
                            uint16_t y4)
{
    g.fillTriangle(x1, y1, x2, y2, x3, y3);
    g.fillTriangle(x2, y2, x3, y3, x4, y4);
}

uint8_t Knob::getQuadrant(float angle)
{
    return ((int)(floor((angle / 1.571f) - 1)) % 4);
}
