#pragma once

#include "Component.h"

class BottomBar : virtual public Component
{
private:
    // \todo move to common config file
    static constexpr uint8_t maxPresetNameLength = 20;
    static constexpr uint8_t maxPageNameLength = 20;
    static constexpr uint8_t maxInfoTextLength = 20;

public:
    BottomBar(const char *newPresetName, const char *newPageName);
    virtual ~BottomBar() override = default;

    void paint(Graphics &g);
    void setPresetName(const char *newName);
    void setPageName(const char *newName);
    void setInfoText(const char *newText);
    void update(const char *newPresetName, const char *newPageName);

private:
    char presetName[maxPresetNameLength + 1];
    char pageName[maxPageNameLength + 1];
    char infoText[maxInfoTextLength + 1];
};
