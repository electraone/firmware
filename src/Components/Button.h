#pragma once

#include "Label.h"

class Button : virtual public Component
{
public:
    Button()
        : highlighted(false),
          disabled(false),
          selected(false),
          colour(0x0001),
          selectedColour(0x0004),
          radius(5)
    {
        label[0] = '\0';
    }

    Button(const char *newLabel,
           uint32_t newColour,
           uint32_t newSelectedColour,
           uint8_t newRadius)
        : highlighted(false),
          disabled(false),
          selected(false),
          colour(newColour),
          selectedColour(newSelectedColour),
          radius(newRadius)
    {
        setLabel(newLabel);
    }

    virtual ~Button() = default;

    std::function<void(void)> onClick;
    std::function<bool(void)> onRelease;

    void setLabel(const char *newLabel)
    {
        if (strlen(newLabel) == 0) {
            *label = '\0';
        } else {
            copyString(label, newLabel, maxLabelLength);
        }
        repaint();
    }

    const char *getLabel(void)
    {
        return (label);
    }

    void setColour(uint32_t newColour)
    {
        colour = newColour;
        repaint();
    }

    uint32_t getColour(void) const
    {
        return (colour);
    }

    void setSelectedColour(uint32_t newColour)
    {
        selectedColour = newColour;
        repaint();
    }

    uint32_t getSelectedColour(void) const
    {
        return (selectedColour);
    }

    void setColours(uint32_t newColour, uint32_t newSelectedColour)
    {
        colour = newColour;
        selectedColour = newSelectedColour;
        repaint();
    }

    void setRadius(uint8_t newRadius)
    {
        radius = newRadius;
        repaint();
    }

    uint8_t getRadius(void) const
    {
        return (radius);
    }

    void setDisabled(bool shouldBeDisabled)
    {
        disabled = shouldBeDisabled;
        repaint();
    }

    bool isDisabled(void) const
    {
        return (disabled);
    }

    void setSelected(bool shouldBeSelected)
    {
        selected = shouldBeSelected;
        repaint();
    }

    bool isSelected(void) const
    {
        return (selected);
    }

    void onTouchDown(const TouchEvent &touchEvent) override
    {
        if (onClick) {
            setHighlighted(true);
        }
    }

    void onTouchUp(const TouchEvent &touchEvent) override
    {
        if (highlighted) {
            setHighlighted(false);
        }

        if (onClick) {
            onClick();
        }

        if (onRelease) {
            onRelease();
        }
    }

    void onPotTouchDown(const PotEvent &potEvent) override
    {
        if (onClick) {
            setHighlighted(true);
        }
    }

    void onPotTouchUp(const PotEvent &potEvent) override
    {
        if (highlighted) {
            setHighlighted(false);
        }

        if (onClick) {
            onClick();
        }

        if (onRelease) {
            onRelease();
        }
    }

    void paint(Graphics &g) override
    {
        uint16_t backgroundColour = colour;

        if (selected) {
            backgroundColour = selectedColour;
        }

        if (highlighted) {
            backgroundColour = Colours::lighter(backgroundColour, 0.1f);
        }

        g.setColour(backgroundColour);
        g.fillRoundRect(0, 0, getWidth(), getHeight(), radius);
        g.printText(0,
                    getHeight() / 2 - 7,
                    label,
                    TextStyle::smallTransparent,
                    getWidth(),
                    TextAlign::center);
        if (dimmed == true) {
            g.dimAll();
        }
    }

private:
    void setHighlighted(bool shouldBeHighligthed)
    {
        highlighted = shouldBeHighligthed;
        repaint();
    }

    static const uint8_t maxLabelLength = 20;
    char label[maxLabelLength + 1];

    uint16_t colour;
    uint16_t selectedColour;
    uint8_t radius;

    struct {
        bool highlighted : 1;
        bool disabled : 1;
        bool selected : 1;
    };
};
