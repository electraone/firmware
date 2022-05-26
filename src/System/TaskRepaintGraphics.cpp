#include "TaskRepaintGraphics.h"
#include "CircularBuffer.h"
#include "Component.h"
#include "Hardware.h"
#include "System.h"

CircularBuffer<Component *, 500> repaintQueue;

/*
 * An option for visual layout debugging
 * \todo to be removed.
 */
bool showComponentFrames = false;

/*
 * repaint graphics
 */
void repaintGraphics(void)
{
    //logMessage("repaintGraphics");
    if (repaintQueue.isEmpty() != true) {
        Hardware::screen.switchWriteLayer();

        while (repaintQueue.isEmpty() != true) {
            Component *component = repaintQueue.shift();

            auto window = component->getWindow();

            if (window == nullptr
                || window == System::windowManager.getActiveWindow()) {
                component->paintWithChildren(Hardware::screen);
                component->painted();
            }
        }

        Hardware::screen.showPreparedLayer(0, 0, 1024, 600);
    }
}
