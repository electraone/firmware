#include "BottomBar.h"
#include "helpers.h"

BottomBar::BottomBar(const char *newPresetName, const char *newPageName)
{
    setName("bottomBar");
    setPresetName(newPresetName);
    setPageName(newPageName);
}

void BottomBar::paint(Graphics &g)
{
    g.fillAll(Colours::black);
    g.setColour(0x2104);
    g.drawLine(8, 0, 1016, 0);
    g.printText(
        0, 4, presetName, TextStyle::smallWhiteOnBlack, 508, TextAlign::right);
    g.printText(
        520, 4, pageName, TextStyle::smallWhiteOnBlack, 508, TextAlign::left);
    g.setColour(Colours::white);
    g.fillRect(512, 9, 2, 2);
    g.dim(0, 3, 1024, 14, Colours::black);
}

void BottomBar::setPresetName(const char *newName)
{
    copyString(presetName, newName, maxPresetNameLength);
}

void BottomBar::setPageName(const char *newName)
{
    copyString(pageName, newName, maxPageNameLength);
}

void BottomBar::update(const char *newPresetName, const char *newPageName)
{
    setPresetName(newPresetName);
    setPageName(newPageName);
    repaint();
}
