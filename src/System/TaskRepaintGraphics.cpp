#include "TaskRepaintGraphics.h"
#include "RepaintQueue.h"
#include "Component.h"
#include "Hardware.h"
#include "System.h"

/*
 * repaint graphics
 */
void repaintGraphics(void)
{
    //logMessage("repaintGraphics");

    System::repaintActive = true;

    if (repaintQueue.isEmpty() != true) {
        Hardware::screen.switchWriteLayer();

        uint16_t i = 0;

        while (repaintQueue.isEmpty() != true) {
            Component *component = repaintQueue.shift();

            // skip identical entries
            if (repaintQueue.size() > 0 && component == repaintQueue[++i]) {
                continue;
            }

            auto window = component->getWindow();

            if (window == nullptr
                || window == System::windowManager.getActiveWindow()) {
                component->paintWithChildren(Hardware::screen);
                component->painted();
            }
        }
        Hardware::screen.showPreparedLayer(0, 0, 1024, 600);
    }
    System::repaintActive = false;
}
