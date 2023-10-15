#pragma once

#include "Hardware.h"
#include "SystemTasks.h"
#include "WindowManager.h"
#include "RepaintManager.h"
#include "Context.h"
#include "RuntimeInfo.h"
#include "Logger.h"

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

class System
{
public:
    System() = delete;

    static void initialise(void);

    static SystemTasks tasks;
    static WindowManager windowManager;
    static RepaintManager repaintManager;
    static Context context;
    static RuntimeInfo runtimeInfo;
    static Logger logger;
    static lua_State *L;

    static bool repaintActive;
    static bool sysExBusy;
};
