#pragma once

#include "Component.h"

class BottomBar : virtual public Component
{
private:
    // \todo move to common config file
    static const uint8_t maxPresetNameLength = 20;
    static const uint8_t maxPageNameLength = 20;

public:
    BottomBar(const char *newPresetName, const char *newPageName);
    virtual ~BottomBar() override = default;

    void paint(Graphics &g);
    void update(const char *newPresetName, const char *newPageName);

private:
    void setPresetName(const char *newName);
    void setPageName(const char *newName);

    char presetName[maxPresetNameLength + 1];
    char pageName[maxPageNameLength + 1];
};
