#include "TaskRefreshMidiIndicators.h"
#include "App.h"

/*
 * update visual status of MIDI indicators on the screen
 */
void refreshMidiIndicators(void)
{
    //logMessage ("refreshMidiIndicators");

    App *app = App::get();

    if (app->statusBar.repaintIsNeeded()) {
        app->statusBar.repaint();
    }
}
