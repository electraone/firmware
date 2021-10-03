#include "helpers.h"
#include "Hardware.h"

Pots::Pots()
{
}

Pot &Pots::operator[](const uint8_t id)
{
    return (pots[id]);
}

Pot *Pots::getNextActivePot(void)
{
    static uint8_t currentPotId = 0;
    uint8_t indexToScan;

    Pot *pot = NULL;

    for (uint8_t i = 0; i < NR_OF_HW_POTS; i++) {
        indexToScan = (currentPotId + i) % NR_OF_HW_POTS;
        if (pots[indexToScan].active == true) {
            pot = &pots[indexToScan];
            break;
        }
    }
    currentPotId = indexToScan + 1;

    if (currentPotId > (MAX_POT_ID)) {
        currentPotId = 0;
    }

    return (pot);
}

Pot *Pots::getNextPot(void)
{
    static uint8_t currentPotId = 0;

    Pot *pot = &pots[currentPotId];

    currentPotId++;

    if (currentPotId >= (MAX_POT_ID)) {
        currentPotId = 0;
    }

    return (pot);
}

void Pots::rescanAllPots(void)
{
    for (uint8_t i = 0; i < NR_OF_HW_POTS; i++) {
        pots[i].initialise();
    }
}

void Pots::initialise(void)
{
  // Initialise Pot Touch
  Hardware::potTouch.begin();
  Hardware::potTouch.reset();

  // Configure Pot Touch
  if (!Hardware::potTouch.init()) {
      logMessage(
          "capacitive sensing: cannot calibrate. Check input USB voltage");
  } else {
      logMessage("capacitive sensing: Initialized");
  }

  // Run the first scan of Pots to get them to known state
  rescanAllPots();
}
