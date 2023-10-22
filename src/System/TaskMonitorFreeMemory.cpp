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

#include "TaskMonitorFreeMemory.h"
#include "Hardware.h"
#include "System.h"
#include "compat.h"

void monitorFreeMemory(void)
{
    Hardware::ram.run();
    System::logger.write(
        LOG_TRACE,
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
