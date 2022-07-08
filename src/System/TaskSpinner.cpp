#include "TaskSpinner.h"
#include "helpers.h"
#include "App.h"

void spinnerTick(void)
{
    if (!Hardware::screen.isStateLocked()) {
        Hardware::screen.setMemoryMode(RA8876::MemoryMode::Bpp16);
        App::get()->spinner.repaint();
        System::tasks.flushRepaintGraphics();
    }
}

void spinnerReset(void)
{
    App::get()->spinner.reset();
    spinnerTick();
}
