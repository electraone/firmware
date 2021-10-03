#include "Graphics.h"

void displaySplash(Graphics &g,
                   const char *appName,
                   const char *appVersion,
                   uint8_t hwRevision)
{
    char versionText[100];

    sprintf(versionText,
            "HW V%d.%d %s %s",
            hwRevision / 100,
            hwRevision - ((hwRevision / 100) * 100),
            appName,
            appVersion);

    g.switchWriteLayer();
    g.setTextColor(COLOR_WHITE);
    g.printText(0,
                360,
                versionText,
                TextStyle::smallWhiteOnBlack,
                1024,
                TextAlign::center);
    g.bteCopy(FRAME_UI_TOOLKIT, 822, 0, FRAME_BUFFER_2, 407, 230, 199, 73);
    g.printText(0,
                500,
                "WWW.ELECTRA.ONE",
                TextStyle::smallWhiteOnBlack,
                1024,
                TextAlign::center);
    g.showPreparedLayer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}
