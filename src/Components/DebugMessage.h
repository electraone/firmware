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
        g.fillAll(Colours::black);
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
