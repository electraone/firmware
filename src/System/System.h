#pragma once

#include "SystemTasks.h"
#include "WindowManager.h"
#include "Context.h"
#include "RuntimeInfo.h"

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
    static Context context;
    static RuntimeInfo runtimeInfo;
    static lua_State *L;
};
