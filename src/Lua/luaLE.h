#pragma once

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

extern lua_State *L;

void luaLE_openEoslibs(lua_State *L, const luaL_Reg *libs);
void luaLE_pushTableString(lua_State *L, const char *key, const char *value);
void luaLE_pushTableInteger(lua_State *L, const char *key, int value);
void luaLE_pushArrayInteger(lua_State *L, int key, int value);
void luaLE_pushArrayString(lua_State *L, int key, const char *value);
void luaLE_pushArrayObject(lua_State *L,
                           int key,
                           const char *objectName,
                           void *object);
void luaLE_pushObject(lua_State *L, const char *objectName, void *object);
void luaLE_getModuleFunction(lua_State *L,
                             const char *module,
                             const char *function);
void luaLE_handleNonexistentFunction(lua_State *L, const char *function);
void luaLE_addObjectMethods(lua_State *L, const luaL_Reg *l);
void luaLE_dumpstack(lua_State *L);
