#include "BottomBar.h"
#include "helpers.h"

BottomBar::BottomBar(const char *newPresetName, const char *newPageName)
{
    setName("bottomBar");
    setPresetName(newPresetName);
    setPageName(newPageName);
    setInfoText("");
}

void BottomBar::paint(Graphics &g)
{
    g.setColour(Colours::white);
    g.fillRoundRect(0, 0, getWidth(), getHeight(), 10);

    g.printText(
        0, 3, presetName, TextStyle::mediumBlackOnWhite, 508, TextAlign::right);
    g.printText(
        520, 3, pageName, TextStyle::mediumBlackOnWhite, 508, TextAlign::left);
    g.printText(
        800, 3, infoText, TextStyle::mediumBlackOnWhite, 180, TextAlign::right);
    g.setColour(Colours::black);
    g.fillRect(512, 9, 4, 4);
    g.dimAll();
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

void BottomBar::update(const char *newPresetName, const char *newPageName)
{
    setPresetName(newPresetName);
    setPageName(newPageName);
    repaint();
}
