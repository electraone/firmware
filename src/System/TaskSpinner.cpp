#include "TaskSpinner.h"
#include "helpers.h"
#include "App.h"

void spinnerTick(void)
{
    App::get()->spinner.repaint();
    System::tasks.flushRepaintGraphics();
}

void spinnerReset(void)
{
    App::get()->spinner.reset();
    spinnerTick();
}
