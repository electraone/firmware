#pragma once

#include "App.h"

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

int sysexBlock_create(lua_State *L);
int sysexBlock_delete(lua_State *L);
int sysexBlock_close(lua_State *L);
int sysexBlock_write(lua_State *L);
int sysexBlock_read(lua_State *L);
int sysexBlock_seek(lua_State *L);
int sysexBlock_peek(lua_State *L);
int sysexBlock_getLength(lua_State *L);
int sysexBlock_getManufacturerSysexId(lua_State *L);

void sysexBlock_register(lua_State *L);

static const luaL_Reg sysexBlock_functions[] = {
    { "close", sysexBlock_close },
    { "write", sysexBlock_write },
    { "read", sysexBlock_read },
    { "seek", sysexBlock_seek },
    { "peek", sysexBlock_peek },
    { "getLength", sysexBlock_getLength },
    { "getManufacturerSysexId", sysexBlock_getManufacturerSysexId },
    { NULL, NULL }
};
