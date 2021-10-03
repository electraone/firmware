#pragma once

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

int luaopen_window(lua_State *L);

int window_addAndMakeVisible(lua_State *L);
int window_repaint(lua_State *L);
int window_clear(lua_State *L);
int window_findChildById(lua_State *L);

static const luaL_Reg window_functions[] = {
    { "addAndMakeVisible", window_addAndMakeVisible },
    { "clear", window_clear },
    { "repaint", window_repaint },
    { "findChildById", window_findChildById },
    { NULL, NULL }
};
