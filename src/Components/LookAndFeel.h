#pragma once

#include "Graphics.h"
#include "ListData.h"

class LookAndFeel
{
public:
    static uint32_t backgroundColour;
    static uint32_t altBackgroundColour;

    static void paintBarHorizontal(Graphics &g,
                                   const Rectangle &bounds,
                                   uint32_t colour,
                                   int16_t min,
                                   int16_t max,
                                   int16_t val);

    static void paintBarVertical(Graphics &g,
                                 const Rectangle &bounds,
                                 uint32_t colour,
                                 int16_t min,
                                 int16_t max,
                                 int16_t val);

    static void paintPad(Graphics &g,
                         const Rectangle &bounds,
                         uint32_t colour,
                         bool isMomentary,
                         bool state);

    static void paintSliderHorizontal(Graphics &g,
                                      const Rectangle &bounds,
                                      uint32_t colour,
                                      int16_t min,
                                      int16_t max,
                                      int16_t val);

    static void paintSliderVertical(Graphics &g,
                                    const Rectangle &bounds,
                                    uint32_t colour,
                                    int16_t min,
                                    int16_t max,
                                    int16_t val);

    static void paintMeterHorizontal(Graphics &g,
                                     const Rectangle &bounds,
                                     uint32_t colourActive,
                                     uint32_t colourInactive,
                                     uint32_t colourBackground,
                                     int16_t min,
                                     int16_t max,
                                     int16_t val);

    static void paintList(Graphics &g,
                          const Rectangle &bounds,
                          uint32_t colour,
                          const ListData *items,
                          uint16_t activeIndex);

    static void paintButtonList(Graphics &g,
                                const Rectangle &bounds,
                                uint32_t colour,
                                const ListData *items,
                                uint16_t activeIndex,
                                bool active);

    static void paintSet(Graphics &g,
                         const Rectangle &bounds,
                         uint32_t colour,
                         const char *label);

    static void paintEnvelope(Graphics &g,
                              const Rectangle &bounds,
                              uint32_t colour,
                              uint16_t baselineY,
                              const std::vector<Point> &points,
                              uint8_t activeSegment,
                              bool showActiveSegment);

private:
    static void paintEnvelopeContour(Graphics &g,
                                     const Rectangle &bounds,
                                     uint32_t colour,
                                     const std::vector<Point> &points);

    static void paintEnvelopeBaseline(Graphics &g,
                                      const Rectangle &bounds,
                                      uint32_t colour,
                                      uint16_t baselineY);

    static void paintEnvelopeMarkers(Graphics &g,
                                     const Rectangle &bounds,
                                     uint32_t colour,
                                     uint16_t baselineY,
                                     const std::vector<Point> &points);

    static void paintEnvelopeFills(Graphics &g,
                                   const Rectangle &bounds,
                                   uint32_t colour,
                                   uint16_t baselineY,
                                   const std::vector<Point> &points,
                                   uint8_t activeSegment,
                                   bool showActiveSegment);

    static bool findIntersection(uint16_t lineY,
                                 const Point &C,
                                 const Point &D,
                                 Point &intersection);

    static void paintDots(Graphics &g,
                          const Rectangle &bounds,
                          uint32_t colour,
                          const ListData *items,
                          uint16_t activeIndex);

    static void paintBar(Graphics &g,
                         const Rectangle &bounds,
                         uint32_t colour,
                         const ListData *items,
                         uint16_t activeIndex);

    // Slider
    static constexpr uint8_t indicatorSize = 22;
    static constexpr uint8_t indicatorRadius = indicatorSize / 2;

    // Set
    static constexpr uint8_t MaxSetLabelLength = 40;
};
