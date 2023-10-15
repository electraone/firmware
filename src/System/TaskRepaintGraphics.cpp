#include "TaskRepaintGraphics.h"
#include "Component.h"
#include "Hardware.h"
#include "System.h"

/*
 * repaint graphics
 */
void repaintGraphics(void)
{
    System::repaintActive = true;
    System::repaintManager.processRepaintQueue();
    System::repaintActive = false;
}
