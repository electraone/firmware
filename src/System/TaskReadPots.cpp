/*
* Electra One MIDI Controller Firmware
* See COPYRIGHT file at the top of the source tree.
*
* This product includes software developed by the
* Electra One Project (http://electra.one/).
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*/

#include "TaskReadPots.h"
#include "Hardware.h"
#include "System.h"
#include "PotListener.h"

/*
 * read HW control
 */
void readPots(void)
{
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
        System::logger.write(LOG_ERROR,
                             "readPots: pot moved: potId=%d, step=%d",
                             pot->id,
                             pot->step);
#endif /* DEBUG */
        PotListener::potBroadcaster.potChange(pot->id, pot->step);
    }
}
