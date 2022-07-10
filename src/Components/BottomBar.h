#pragma once

#include "Component.h"
#include "MeterHorizontal.h"

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

    void paint(Graphics &g) override;
    void resized(void) override;

    void setPresetName(const char *newName);
    void setPageName(const char *newName);
    void setInfoText(const char *newText);
    void setRamPercentage(uint8_t newPercentage);
    void update(const char *newPresetName, const char *newPageName);

private:
    char presetName[maxPresetNameLength + 1];
    char pageName[maxPageNameLength + 1];
    char infoText[maxInfoTextLength + 1];

    MeterHorizontal *ramIndicator;
};
