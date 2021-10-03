#pragma once

#define _TASK_MICRO_RES
#include "TaskSchedulerDeclarations.h"
#include <IntervalTimer.h>

#include "TaskReadButtons.h"
#include "TaskReadPots.h"
#include "TaskReadPotTouch.h"
#include "TaskReadLCDTouch.h"
#include "TaskScanUSBHost.h"
#include "TaskRefreshMidiIndicators.h"
#include "TaskRepaintGraphics.h"
#include "TaskRunTimer.h"
#include "TaskRunUserTask.h"
#include "TaskMonitorFreeMemory.h"
#include "TaskReadMidi.h"
#include "TaskSendMidi.h"

class SystemTasks : public Scheduler
{
public:
    SystemTasks();
    ~SystemTasks() = default;

    void enableAll(void);
    void enableMidi(void);
    void enableCtrlMidi(void);
    void disableMidi(void);
    void enableTimer(void);
    void disableTimer(void);
    bool isTimerEnabled(void);
    unsigned long timerGetInterval(void);
    void timerSetInterval(unsigned long interval);
    void doNotUsePotTouch(void);
    void enableUserTask(void);
    void disableUserTask(void);

private:
    IntervalTimer timerReadMidi;

    Task taskMonitorFreeMemory;
    Task taskReadPots;
    Task taskReadButtons;
    Task taskReadPotTouch;
    Task taskReadLCDTouch;
    Task taskScanUSBHost;
    Task taskRefreshMidiIndicators;
    Task taskRepaintGraphics;
    Task taskUserTask;
    Task taskRunTimer;
    Task taskSendMidi;
    Task taskProcessMidi;
};
