#include "TaskMonitorFreeMemory.h"
#include "Hardware.h"
#include "helpers.h"

void monitorFreeMemory(void)
{
    //logMessage ("monitorFreeMemory");
    Hardware::ram.run();
    logMessage("free RAM: free=%d, heap=%d, collision=%d",
               Hardware::ram.adj_free(),
               Hardware::ram.heap_used(),
               Hardware::ram.warning_crash());
}
