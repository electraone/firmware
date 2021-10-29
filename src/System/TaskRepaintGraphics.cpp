#include "TaskRepaintGraphics.h"
#include "CircularBuffer.h"
#include "Component.h"
#include "Hardware.h"

CircularBuffer<Component *, 100> repaintQueue;

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
    //logMessage ("repaintGraphics");
    if (repaintQueue.isEmpty() != true) {
        logMessage("in");
        Hardware::screen.switchWriteLayer();

        while (repaintQueue.isEmpty() != true) {
            Component *component = repaintQueue.shift();
            component->paintWithChildren(Hardware::screen);
            component->painted();
        }

        Hardware::screen.showPreparedLayer(0, 0, 1024, 600);
        logMessage("out");
    }
}
