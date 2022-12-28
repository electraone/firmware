#include "SystemTasks.h"
#include "TaskScheduler.h"
#include "System.h"
#include "RepaintQueue.h"
#include "UpdateModeScreen.h"

SystemTasks::SystemTasks()
    : taskMonitorFreeMemory(10000000, TASK_FOREVER, &monitorFreeMemory),
      taskCollectGarbage(10000000, TASK_FOREVER, &collectGarbage),
      taskReadPots(10000, TASK_FOREVER, &readPots),
      taskReadButtons(15000, TASK_FOREVER, &readButtons),
      taskReadPotTouch(100000, TASK_FOREVER, &readPotTouch),
      taskReadLCDTouch(40000, TASK_FOREVER, &readLCDTouch),
      taskScanUSBHost(2000000, TASK_FOREVER, &scanUSBHost),
      taskRefreshMidiIndicators(100000, TASK_FOREVER, &refreshMidiIndicators),
      taskRepaintGraphics(40000, TASK_FOREVER, &repaintGraphics),
      taskUserTask(250000, TASK_FOREVER, &userTask),
      taskRunTimer(500000, TASK_FOREVER, &runTimer),
      taskSendMidi(2000, TASK_FOREVER, &sendMidi),
      taskProcessMidi(5000, TASK_FOREVER, &processMidi),
      taskProcessSysex(5000, TASK_FOREVER, &processSysex)
{
    init();
    addTask(taskMonitorFreeMemory);
    addTask(taskCollectGarbage);
    addTask(taskReadPots);
    addTask(taskReadButtons);
    addTask(taskReadLCDTouch);
    addTask(taskScanUSBHost);
    addTask(taskRefreshMidiIndicators);
    addTask(taskReadPotTouch);
    addTask(taskRepaintGraphics);
    addTask(taskUserTask);
    addTask(taskRunTimer);
    addTask(taskSendMidi);
    addTask(taskProcessMidi);
    addTask(taskProcessSysex);
}

void SystemTasks::enableAll()
{
    taskMonitorFreeMemory.disable();
    taskCollectGarbage.enable();
    taskReadPots.enable();
    taskReadButtons.enable();
    taskReadLCDTouch.enable();
    taskScanUSBHost.enable();
    taskRefreshMidiIndicators.enable();
    timerReadMidi.priority(150);
    timerSpinner.priority(255);

    if (System::context.getTouchEnabled()) {
        taskReadPotTouch.enable();
    } else {
        taskReadPots.setInterval(2500);
    }

    taskRepaintGraphics.enable();
    taskUserTask.disable();
    taskRunTimer.disable();
    taskSendMidi.enable();
    taskProcessMidi.enable();
    taskProcessSysex.enable();
}

void SystemTasks::enableSleepMode()
{
    taskMonitorFreeMemory.disable();
    taskCollectGarbage.disable();
    taskReadPots.disable();
    taskReadButtons.enable();
    taskReadLCDTouch.disable();
    taskScanUSBHost.disable();
    taskRefreshMidiIndicators.disable();
    timerReadMidi.priority(150);
    timerSpinner.priority(255);
    taskReadPotTouch.disable();
    taskRepaintGraphics.disable();
    taskUserTask.disable();
    taskRunTimer.disable();
    taskSendMidi.disable();
    taskProcessMidi.disable();
    taskProcessSysex.disable();
    disableMidi();
}

void SystemTasks::enableMidi(void)
{
    timerReadMidi.begin(readMidi, 500);
}

void SystemTasks::enableCtrlMidi(void)
{
    timerReadMidi.end();
    timerReadMidi.begin(readCtrlMidi, 6);
}

void SystemTasks::disableMidi(void)
{
    timerReadMidi.end();
}

void SystemTasks::enableTimer(void)
{
    taskRunTimer.enable();
}

void SystemTasks::disableTimer(void)
{
    taskRunTimer.disable();
}

bool SystemTasks::isTimerEnabled(void)
{
    return (taskRunTimer.isEnabled());
}

unsigned long SystemTasks::timerGetInterval(void)
{
    return (taskRunTimer.getInterval());
}

void SystemTasks::timerSetInterval(unsigned long interval)
{
    taskRunTimer.setInterval(interval);
}

void SystemTasks::enableRepaintGraphics(void)
{
    taskRepaintGraphics.enable();
}

void SystemTasks::disableRepaintGraphics(void)
{
    taskRepaintGraphics.disable();
}

void SystemTasks::flushRepaintGraphics(void)
{
    repaintGraphics();
}

void SystemTasks::clearRepaintGraphics(void)
{
    repaintQueue.clear();
}

void SystemTasks::doNotUsePotTouch(void)
{
    taskReadPotTouch.disable();
    taskReadPots.setInterval(2500);
}

void SystemTasks::enableUserTask(void)
{
    taskUserTask.enable();
}

void SystemTasks::disableUserTask(void)
{
    taskUserTask.disable();
}

void SystemTasks::enableSpinner(void)
{
    clearRepaintGraphics();
    spinnerReset();
    timerSpinner.begin(spinnerTick, 100000);
}

void SystemTasks::disableSpinner(void)
{
    timerSpinner.end();
}

void SystemTasks::displayUpdateModeScreen(void)
{
    UpdateModeScreen ums;
    clearRepaintGraphics();
    ums.repaint();
    flushRepaintGraphics();
}
