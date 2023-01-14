#include "TaskMonitorFreeMemory.h"
#include "Hardware.h"
#include "System.h"
#include "wiring.h"

void monitorFreeMemory(void)
{
    //logMessage ("monitorFreeMemory");
    Hardware::ram.run();
    System::logger.write(
        TRACE,
        "free RAM: total=%d, adj_free=%d, free=%d, heap=%d, collision=%d",
        Hardware::ram.total(),
        Hardware::ram.adj_free(),
        Hardware::ram.free(),
        Hardware::ram.heap_used(),
        Hardware::ram.warning_crash());
}

int getUsedMemory(void)
{
    // 185000 is memory available on top of the base system.
    // \todo - sample the actual value after the system is initialized
    return (
        constrain(100 - (Hardware::ram.adj_free() / (185000 / 100)), 0, 100));
}
