#include "TaskCollectGarbage.h"
#include "helpers.h"

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

extern lua_State *L;

void collectGarbage(void)
{
    //logMessage ("collectGarbage");
    if (L) {
        lua_gc(L, LUA_GCCOLLECT, 0);
    }
}
