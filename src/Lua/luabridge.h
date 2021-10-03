#pragma once

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "App.h"
#include "MidiMessage.h"
#include "luaLE.h"
#include "luaSysexBlock.h"
#include "luaMidi.h"
#include "luaTimer.h"
#include "luaTransport.h"
#include "luaWindow.h"

extern lua_State *L;

extern "C" {
int lua_compat_fopen(const char *filename);
void lua_compat_fclose();
char lua_compat_getc();
int lua_compat_feof();
size_t lua_compat_fread(void *ptr, size_t size, size_t count);
int lua_compat_ferror();
void lua_compat_printf(char *format, ...);
void lua_compat_print(const char *s);
}

void loadLuaModule(const char *filename);
void runLuaFunction(const char *functionName);
void runLuaString(const char *commandText);

void initLua(void);
void closeLua(void);
void executeElectraLua(const char *filename);
bool isLuaValid(const char *filename);

void loadGlobalVariables(lua_State *L);
