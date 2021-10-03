#include "luaTimer.h"
#include "luaLE.h"
#include "System.h"

int luaopen_timer(lua_State *L)
{
    luaL_newlib(L, timer_functions);
    return 1;
}

int timer_enable(lua_State *L)
{
    lua_settop(L, 0);
    System::tasks.enableTimer();

    return (0);
}

int timer_disable(lua_State *L)
{
    lua_settop(L, 0);
    System::tasks.disableTimer();

    return (0);
}

int timer_isEnabled(lua_State *L)
{
    lua_pushboolean(L, System::tasks.isTimerEnabled());
    return (1);
}

int timer_setPeriod(lua_State *L)
{
    lua_settop(L, 1);

    int period = luaL_checkinteger(L, -1);

    if (period < 10) {
        return (luaL_error(L, "Minimum timer period is 10 mSec"));
    }

    System::tasks.timerSetInterval(period * 1000);

    return (0);
}

int timer_getPeriod(lua_State *L)
{
    unsigned long interval = System::tasks.timerGetInterval();

    lua_pushinteger(L, interval / 1000);
    return (1);
}

int timer_setBpm(lua_State *L)
{
    lua_settop(L, 1);

    int bmp = luaL_checkinteger(L, -1);

    if (bmp > 6000) {
        return (luaL_error(L, "Maximum timer BMP is 6000"));
    }

    System::tasks.timerSetInterval(60000000 / bmp);

    return (0);
}

int timer_getBpm(lua_State *L)
{
    unsigned long interval = System::tasks.timerGetInterval();

    lua_pushinteger(L, 60000000 / interval);
    return (1);
}

void timer_runCallback(void)
{
    luaLE_getModuleFunction(L, "timer", "onTick");

    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != 0) {
            logMessage("error running function 'timer.callback': %s",
                       lua_tostring(L, -1));
        }
    } else {
        luaLE_handleNonexistentFunction(L, "onTick");
    }
}
