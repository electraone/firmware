#pragma once

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "helpers.h"

int luaopen_timer(lua_State *L);

int timer_enable(lua_State *L);
int timer_disable(lua_State *L);
int timer_isEnabled(lua_State *L);
int timer_setPeriod(lua_State *L);
int timer_getPeriod(lua_State *L);
int timer_setBpm(lua_State *L);
int timer_getBpm(lua_State *L);
void timer_runCallback(void);

static const luaL_Reg timer_functions[] = {
    { "enable", timer_enable },       { "disable", timer_disable },
    { "isEnabled", timer_isEnabled }, { "setPeriod", timer_setPeriod },
    { "getPeriod", timer_getPeriod }, { "setBpm", timer_setBpm },
    { "getBpm", timer_getBpm },       { NULL, NULL }
};
