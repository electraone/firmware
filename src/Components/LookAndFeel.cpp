#include "LookAndFeel.h"

void LookAndFeel::paintBarHorizontal(Graphics &g,
                                     const Rectangle &bounds,
                                     uint32_t colour,
                                     int16_t min,
                                     int16_t max,
                                     int16_t val)
{
    uint32_t colourTrack = Colours::darker(colour, 0.3f);

    uint16_t barHeight = bounds.getHeight() * 1.0f;
    uint16_t padding = (bounds.getHeight() - barHeight) / 2;

    uint16_t barX = map(0, min, max, 0, bounds.getWidth());
    uint16_t barWidth = map(val, min, max, 0, bounds.getWidth()) - barX;

    // Clear the component area
    g.fillAll(Colours::black);

    // Paint the track background
    g.setColour(colourTrack);
    g.fillRect(0, padding, bounds.getWidth(), barHeight);

    // Paint the active bar
    g.setColour(colour);
    g.fillRect(barX, padding, barWidth, barHeight);

    // Paint the outline
    g.drawRect(0, padding, bounds.getWidth(), barHeight);
}

void LookAndFeel::paintBarVertical(Graphics &g,
                                   const Rectangle &bounds,
                                   uint32_t colour,
                                   int16_t min,
                                   int16_t max,
                                   int16_t val)
{
    uint32_t colourTrack = Colours::darker(colour, 0.3f);

    uint16_t barWidth = bounds.getWidth() * 1.0f;
    uint16_t padding = (bounds.getWidth() - barWidth) / 2;

    uint16_t barY = map(0, min, max, 0, bounds.getHeight());
    uint16_t barHeight = map(val, min, max, 0, bounds.getHeight()) - barY;

    // Clear the component area
    g.fillAll(Colours::black);

    // Paint the track background
    g.setColour(colourTrack);
    g.fillRect(padding, 0, barWidth, bounds.getHeight());

    // Paint the active bar
    g.setColour(colour);
    g.fillRect(padding, bounds.getHeight() - barY, barWidth, -barHeight);

    // Paint the outline
    g.drawRect(padding, 0, barWidth, bounds.getHeight());
}

void LookAndFeel::paintPad(Graphics &g,
                           const Rectangle &bounds,
                           uint32_t colour,
                           bool state)
{
    uint32_t colourOff = Colours::darker(colour, 0.1f);
    uint32_t colourOn = Colours::darker(colour, 0.8f);

    // Paint the background
    g.setColour(state == true ? colourOn : colourOff);
    g.fillRoundRect(0, 0, bounds.getWidth(), bounds.getHeight(), 8);

    // Paint the outline
    g.setColour(colour);
    g.drawRoundRect(0, 0, bounds.getWidth(), bounds.getHeight(), 8);
}

void LookAndFeel::paintSliderHorizontal(Graphics &g,
                                        const Rectangle &bounds,
                                        uint32_t colour,
                                        int16_t min,
                                        int16_t max,
                                        int16_t val)
{
    uint32_t colourTrack = Colours::darker(colour, 0.3f);

    uint16_t padding = (bounds.getHeight() - indicatorSize) / 2;
    uint16_t indicatorX =
        map(val, min, max, 0, bounds.getWidth() - indicatorSize);

    // Clear the component area
    g.fillAll(Colours::black);

    // Slider track
    g.setColour(colourTrack);
    g.fillRect(0, padding + indicatorRadius - 2, bounds.getWidth(), 4);

    // Indicator
    g.setColour(colour);
    g.fillElipse(
        indicatorX + indicatorRadius, padding + indicatorRadius, 11, 11);
}

void LookAndFeel::paintSliderVertical(Graphics &g,
                                      const Rectangle &bounds,
                                      uint32_t colour,
                                      int16_t min,
                                      int16_t max,
                                      int16_t val)
{
    uint32_t colourTrack = Colours::darker(colour, 0.3f);

    uint16_t padding = (bounds.getWidth() - indicatorSize) / 2;
    uint16_t indicatorY =
        map(val, min, max, 0, bounds.getHeight() - indicatorSize);

    // Clear the component area
    g.fillAll(Colours::black);

    // Slider track
    g.setColour(colourTrack);
    g.fillRect(padding + indicatorRadius - 2, 0, 4, bounds.getHeight());

    // Indicator
    g.setColour(colour);
    g.fillElipse(padding + indicatorRadius,
                 bounds.getHeight() - indicatorY - indicatorRadius,
                 11,
                 11);
}

void LookAndFeel::paintList(Graphics &g,
                            const Rectangle &bounds,
                            uint32_t colour,
                            const ListData *items,
                            uint8_t activeIndex)
{
    // Paint the background
    g.fillAll(Colours::black);

    if (items->getByIndex(activeIndex).isBitmapEmpty()) {
        // Print the label
        g.printText(0,
                    0,
                    items->getByIndex(activeIndex).getLabel(),
                    TextStyle::mediumTransparent,
                    bounds.getWidth(),
                    TextAlign::center);
    } else {
        // display bitmap image
        uint16_t paddingBitmap = ((bounds.getWidth() - BITMAP_WIDTH)) / 2 - 1;
        items->getByIndex(activeIndex).paintBitmap(paddingBitmap, 0, colour);
    }
    // Paint the graphics
    if (items->getNumItems() < 16) {
        paintDots(g, bounds, colour, items, activeIndex);
    } else {
        paintBar(g, bounds, colour, items, activeIndex);
    }
}

void LookAndFeel::paintSet(Graphics &g,
                           const Rectangle &bounds,
                           uint32_t colour,
                           const char *label)
{
    uint16_t width = bounds.getWidth();
    uint16_t height = bounds.getHeight();

    // This is just a relaxed approximation
    uint16_t nameLength = width / 8;

    if (nameLength > MaxSetLabelLength) {
        nameLength = MaxSetLabelLength;
    }

    char labelAdjusted[MaxSetLabelLength + 1];

    copyString(labelAdjusted, label, MaxSetLabelLength);

    g.fillAll(Colours::black);

    g.setColour(colour);

    for (uint16_t i = 0; i <= (width / 2); i += 3) {
        g.drawPixel(i, 6);
    }

    for (uint16_t i = width; i >= (width / 2); i -= 3) {
        g.drawPixel(i, 6);
    }

    if (height < 80) {
        g.drawPixel(0, 9);
        g.drawPixel(width, 9);
    } else {
        for (uint16_t i = 6; i < height; i += 3) {
            g.drawPixel(0, i);
            g.drawPixel(width, i);
        }

        for (uint16_t i = 0; i <= (width / 2); i += 3) {
            g.drawPixel(i, height);
        }
        for (uint16_t i = width; i >= (width / 2); i -= 3) {
            g.drawPixel(i, height);
        }
    }

    if (strlen(labelAdjusted) > 0) {
        g.textPlaceHolder(0,
                          0,
                          labelAdjusted,
                          TextStyle::smallWhiteOnBlack,
                          width,
                          TextAlign::center);
        g.printText(0,
                    0,
                    labelAdjusted,
                    TextStyle::smallWhiteOnBlack,
                    width,
                    TextAlign::center);
    }
}

// Envelope -----------------------------------------------------------------

void LookAndFeel::paintEnvelope(Graphics &g,
                                const Rectangle &bounds,
                                uint32_t colour,
                                uint16_t baselineY,
                                const std::vector<Point> &points,
                                uint8_t activeSegment,
                                bool showActiveSegment)
{
    g.fillAll(Colours::black);

    paintEnvelopeFills(
        g, bounds, colour, baselineY, points, activeSegment, showActiveSegment);
    paintEnvelopeMarkers(g, bounds, colour, baselineY, points);
    paintEnvelopeBaseline(g, bounds, colour, baselineY);
    paintEnvelopeContour(g, bounds, colour, points);
}

void LookAndFeel::paintEnvelopeContour(Graphics &g,
                                       const Rectangle &bounds,
                                       uint32_t colour,
                                       const std::vector<Point> &points)
{
    g.setColour(colour);

    for (uint8_t i = 0; i < std::max(0, (int)(points.size() - 1)); i += 1) {
        g.drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }
}

void LookAndFeel::paintEnvelopeBaseline(Graphics &g,
                                        const Rectangle &bounds,
                                        uint32_t colour,
                                        uint16_t baselineY)
{
    g.setColour(Colours::black);
    g.drawLine(0, baselineY, bounds.getWidth(), baselineY);
    g.setColour(Colours::darkgrey);
    g.drawHorizontalDottedLine(0, bounds.getWidth(), baselineY);
}

void LookAndFeel::paintEnvelopeMarkers(Graphics &g,
                                       const Rectangle &bounds,
                                       uint32_t colour,
                                       uint16_t baselineY,
                                       const std::vector<Point> &points)
{
    g.setColour(Colours::black);

    for (uint8_t i = 0; i < std::max(0, (int)(points.size() - 1)); i += 1) {
        g.drawLine(points[i].x, points[i].y, points[i].x, baselineY);
    }
}

void LookAndFeel::paintEnvelopeFills(Graphics &g,
                                     const Rectangle &bounds,
                                     uint32_t colour,
                                     uint16_t baselineY,
                                     const std::vector<Point> &points,
                                     uint8_t activeSegment,
                                     bool showActiveSegment)
{
    uint32_t darker = Colours::darker(colour, 0.2f);

    for (uint8_t i = 0; i < std::max(0, (int)(points.size() - 1)); i++) {
        Point intersection(0, 0);

        if (showActiveSegment
            && (i == (activeSegment + 1))) { // the delay is ignored
            g.setColour(colour);
        } else {
            g.setColour(darker);
        }

        if (findIntersection(
                baselineY, points[i], points[i + 1], intersection)) {
            // The segment intersects with the baseline

            //g.setColour(darker);

            // Paint two wedges
            g.fillTriangle(points[i].x,
                           points[i].y,
                           points[i].x,
                           intersection.y,
                           intersection.x - 1,
                           intersection.y);
            g.fillTriangle(intersection.x,
                           intersection.y,
                           points[i + 1].x - 1,
                           intersection.y,
                           points[i + 1].x - 1,
                           points[i + 1].y);

        } else {
            // The segment does not intersect with the baseline
            uint8_t xi = 0;
            uint8_t yi = 0;

            // Set wedge points based on the quadrant
            if (points[i].y <= baselineY) {
                xi = (points[i].y > points[i + 1].y) ? i + 1 : i;
                yi = (points[i].y > points[i + 1].y) ? i : i + 1;
            } else {
                xi = (points[i].y > points[i + 1].y) ? i : i + 1;
                yi = (points[i].y > points[i + 1].y) ? i + 1 : i;
            }

            //g.setColour(darker);

            // Paint the wedge
            g.fillTriangle(points[i].x,
                           points[i].y,
                           points[i + 1].x - 1,
                           points[i + 1].y,
                           points[xi].x,
                           points[yi].y);

            // Paint the rectangle to fill space between wedge and the baseline
            if ((points[i].y != baselineY) && (points[i + 1].y != baselineY)) {
                g.fillRect(points[i].x,
                           points[yi].y,
                           points[i + 1].x - points[i].x,
                           baselineY - points[yi].y);
            }
        }
    }
}

bool LookAndFeel::findIntersection(uint16_t lineY,
                                   const Point &C,
                                   const Point &D,
                                   Point &intersection)
{
    // Envelope baseline represented as a1x + b1y = c1
    int a1 = 0;
    int b1 = -1024;
    int c1 = b1 * lineY;

    // Line CD represented as a2x + b2y = c2
    int a2 = D.y - C.y;
    int b2 = C.x - D.x;
    int c2 = a2 * C.x + b2 * C.y;

    int determinant = a1 * b2 - a2 * b1;

    if (determinant != 0) {
        if (!(((C.y <= lineY) && (D.y <= lineY))
              || ((C.y > lineY) && (D.y > lineY)))) {
            uint16_t x = (b2 * c1 - b1 * c2) / determinant;
            uint16_t y = (a1 * c2 - a2 * c1) / determinant;
            intersection = Point(x, y);
            return (true);
        }
    }

    return (false);
}

void LookAndFeel::paintDots(Graphics &g,
                            const Rectangle &bounds,
                            uint32_t colour,
                            const ListData *items,
                            uint8_t activeIndex)
{
    // Paint the dots
    uint8_t paddingDots =
        (bounds.getWidth() - (items->getNumItems() * 8)) / 2 + 4;
    uint16_t yPosition = 25;

    // paint dots
    for (uint16_t i = 0; i < items->getNumItems(); i++) {
        g.setColour((i == activeIndex) ? colour : Colours::darker(colour, 0.5));
        g.fillCircle(paddingDots + i * 8, yPosition, 2);
    }
}

void LookAndFeel::paintBar(Graphics &g,
                           const Rectangle &bounds,
                           uint32_t colour,
                           const ListData *items,
                           uint8_t activeIndex)
{
    uint16_t lastItem = items->getMaxIndex();
    uint32_t dark = Colours::darker(colour, 0.5);
    uint32_t light = Colours::lighter(colour, 0.5);

    uint16_t paddingFader = (bounds.getWidth() - 137) / 2;
    float step = 127.0f / (float)lastItem;
    float faderLength = std::max(1.0f, abs(step * (activeIndex)));
    uint16_t yPosition = 25;

    // Paint the track
    g.setColour(dark);
    g.fillRect(paddingFader + 4, yPosition - 2, 129, 5);

    // Paint the start point
    g.setColour((activeIndex == 0) ? light : dark);
    g.fillCircle(paddingFader + 2, yPosition, 2);

    // Paint the active point on the track
    if ((activeIndex != 0) && (activeIndex != lastItem)) {
        g.setColour((activeIndex == lastItem) ? light : dark);
        g.fillCircle(paddingFader + 134, yPosition, 2);
    }

    // Paint the end point
    g.setColour(light);
    g.fillRect(paddingFader + faderLength, yPosition - 2, step, 5);
}
