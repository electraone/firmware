#include "TaskReadPots.h"
#include "Hardware.h"
#include "System.h"
#include "PotListener.h"

/*
 * read HW control
 */
void readPots(void)
{
    //logMessage ("readPots");

    Pot *pot = NULL;

    if (System::context.getTouchEnabled() == false) {
        pot = Hardware::pots.getNextPot();
    } else {
        pot = Hardware::pots.getNextActivePot();
    }

    if (pot == NULL) {
        return;
    }

    pot->process();

    if (pot->step != 0) {
#ifdef DEBUG
        logMessage(
            "readPots: pot moved: potId=%d, step=%d", pot->id, pot->step);
#endif /* DEBUG */
        PotListener::potBroadcaster.potChange(pot->id, pot->step);
    }
}
