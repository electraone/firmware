#include "TaskReadPotTouch.h"
#include "Hardware.h"
#include "PotListener.h"

/*
 * Read capacitive touch
 */
void readPotTouch(void)
{
    //logMessage ("readPotTouch");

    if (digitalRead(CAP_INT_PIN) == HIGH) {
        uint16_t sensors = Hardware::potTouch.readSensors();

        for (uint8_t potId = 0; potId < MAX_POT_ID; potId++) {
            Hardware::pots[potId].touched = ((sensors >> (potId)) & 0x01);

            if (Hardware::pots[potId].touched) {
#ifdef DEBUG
                System::logger.write(
                    "readPotTouch: pot touch touched: potId=%d", potId);
#endif /* DEBUG */

                if (Hardware::pots[potId].active == false) {
                    Hardware::pots[potId].active = true;
                    PotListener::potBroadcaster.potTouchDown(potId);
                }
            } else {
                if (Hardware::pots[potId].active == true) {
#ifdef DEBUG
                    System::logger.write(
                        "readPotTouch: pot touch released: potId=%d", potId);
#endif /* DEBUG */
                    Hardware::pots[potId].active = false;
                    PotListener::potBroadcaster.potTouchUp(potId);
                }
            }
        }
        Hardware::potTouch.clearAllInterrupts();
    }
}
