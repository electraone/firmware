#pragma once

#include "Component.h"
#include "Colours.h"

class FlashMessage : public Component
{
public:
    FlashMessage()
        : text(nullptr),
          style(TextStyle::smallWhiteOnBlack),
          aligment(TextAlign::left)
    {
    }

    FlashMessage(Rectangle bounds,
                 const char *newText,
                 TextStyle newStyle,
                 TextAlign newAligment)
        : text(newText), style(newStyle), aligment(newAligment), isShown(false)
    {
        setBounds(bounds);
    }

    bool getIsShown(void)
    {
        return (isShown);
    }

    void hide(void)
    {
        isShown = false;
        repaint();
    }

    void show(void)
    {
        isShown = true;
        repaint();
    }

    void paint(Graphics &g)
    {
        g.setColour(0x5040);
        g.fillRect(0, 0, getWidth(), getHeight());

        if (isShown) {
            if ((text != nullptr) && (strlen(text) > 0)) {
                g.printText(0,
                            (getHeight() / 2 - 6),
                            text,
                            style,
                            getWidth(),
                            aligment);
            }
        }
    }

private:
    Colour colour;
    const char *text;
    TextStyle style;
    TextAlign aligment;
    bool isShown;
};
