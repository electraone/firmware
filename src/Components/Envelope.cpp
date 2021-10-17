#include "Envelope.h"

Envelope::Envelope()
    : activeSegment(1), boundaryMin(0.0f), boundaryMax(1.0f), baselineY(0)
{
}

void Envelope::setColour(uint32_t newColour)
{
    colour = newColour;
    repaint();
}

void Envelope::setValue(uint8_t handle, float newValue)
{
    values[handle].value =
        constrain(newValue, values[handle].min, values[handle].max);
    repaint();
}

void Envelope::setMin(uint8_t handle, float newMin)
{
    values[handle].min = newMin;
    boundaryMin = (newMin < boundaryMin) ? newMin : boundaryMin;
    setValue(handle, values[handle].value);
}

void Envelope::setMax(uint8_t handle, float newMax)
{
    values[handle].max = newMax;
    boundaryMax = (newMax > boundaryMax) ? newMax : boundaryMax;
    setValue(handle, values[handle].value);
}

void Envelope::setActiveSegment(uint8_t newActiveSegment)
{
    activeSegment = newActiveSegment;
}

void Envelope::onTouchMove(const TouchEvent &touchEvent)
{
}

void Envelope::onTouchDown(const TouchEvent &touchEvent)
{
}

void Envelope::onTouchUp(const TouchEvent &touchEvent)
{
}

void Envelope::onPotChange(const PotEvent &potEvent)
{
    if (potEvent.getRelativeChange() > 0) {
        setValue(activeSegment, values[activeSegment].value + 0.01f);
    } else {
        setValue(activeSegment, values[activeSegment].value - 0.01f);
    }
}

void Envelope::onPotTouchDown(const PotEvent &potEvent)
{
}

void Envelope::onPotTouchUp(const PotEvent &potEvent)
{
}

void Envelope::paint(Graphics &g)
{
    // Clear the envelope area
    g.fillAll(Colours::black);

    // Compute points on the envelope contour
    computePoints();

    paintFills(g);
    paintMarkers(g);
    paintBaseline(g);
    paintContour(g);
}

void Envelope::paintContour(Graphics &g)
{
    g.setColour(colour);

    for (uint8_t i = 0; i < std::max(0, (int)(points.size() - 1)); i += 1) {
        g.drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
    }
}

void Envelope::paintBaseline(Graphics &g)
{
    g.setColour(Colours::black);
    g.drawLine(0, baselineY, getWidth(), baselineY);
    g.setColour(Colours::darkgrey);
    g.drawHorizontalDottedLine(0, getWidth(), baselineY);
}

void Envelope::paintMarkers(Graphics &g)
{
    g.setColour(Colours::black);

    for (uint8_t i = 0; i < std::max(0, (int)(points.size() - 1)); i += 1) {
        g.drawLine(points[i].x, points[i].y, points[i].x, baselineY);
    }
}

void Envelope::paintFills(Graphics &g)
{
    uint32_t darker = Colours::darker(colour, 0.2f);

    for (uint8_t i = 0; i < std::max(0, (int)(points.size() - 1)); i++) {
        Point intersection(0, 0);

        if (findIntersection(
                baselineY, points[i], points[i + 1], intersection)) {
            // The segment intersects with the baseline

            g.setColour(darker);

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

            g.setColour(darker);

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

void Envelope::resized(void)
{
    repaint();
}

bool Envelope::findIntersection(uint16_t lineY,
                                Point &C,
                                Point &D,
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
