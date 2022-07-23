#include "BottomBar.h"
#include "helpers.h"

BottomBar::BottomBar(const char *newPresetName, const char *newPageName)
    : ramIndicator(nullptr)
{
    setName("bottomBar");
    setPresetName(newPresetName);
    setPageName(newPageName);
    setInfoText("");

    ramIndicator = new MeterHorizontal();

    if (ramIndicator) {
        ramIndicator->setColour(0xAD55);
        ramIndicator->setColourInactive(Colours::black);
        ramIndicator->setColourBackground(0x2104);
        ramIndicator->setMinimum(0.0f);
        ramIndicator->setMaximum(100.0f);
        ramIndicator->setValue(0);
        addAndMakeVisible(ramIndicator);
    }
}

void BottomBar::paint(Graphics &g)
{
    g.setColour(0x2104);
    g.fillRoundRect(0, 0, getWidth(), getHeight(), 10);
    g.printText(
        0, 4, presetName, TextStyle::mediumTransparent, 500, TextAlign::right);
    g.printText(
        528, 4, pageName, TextStyle::mediumTransparent, 400, TextAlign::left);
    g.printText(
        800, 4, infoText, TextStyle::mediumTransparent, 180, TextAlign::right);

    g.setColour(Colours::white);
    g.fillRect(512, 9, 2, 2);
}

void BottomBar::resized(void)
{
    ramIndicator->setBounds(20, 8, 50, 8);
}

void BottomBar::setPresetName(const char *newName)
{
    copyString(presetName, newName, maxPresetNameLength);
}

void BottomBar::setPageName(const char *newName)
{
    copyString(pageName, newName, maxPageNameLength);
}

void BottomBar::setInfoText(const char *newText)
{
    copyString(infoText, newText, maxPageNameLength);
}

void BottomBar::setRamPercentage(uint8_t newPercentage)
{
    ramIndicator->setValue(newPercentage);
}

void BottomBar::update(const char *newPresetName, const char *newPageName)
{
    setPresetName(newPresetName);
    setPageName(newPageName);
    repaint();
}
