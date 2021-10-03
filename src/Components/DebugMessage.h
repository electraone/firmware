#pragma once

#include "Component.h"

class DebugMessage : public Component
{
public:
    DebugMessage(char *newBuf) : buf(newBuf)
    {
    }

    ~DebugMessage()
    {
    }

    void paint(Graphics &g)
    {
        g.fillAll(ElectraColours::rgb565NumericBlack);
        g.printText(0,
                    0,
                    buf,
                    TextStyle::smallWhiteOnBlack,
                    getWidth(),
                    TextAlign::left);
    }

private:
    char *buf;
};
