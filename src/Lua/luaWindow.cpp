#include "luaWindow.h"
#include "System.h"
#include "SelectorComponent.h"

int luaopen_window(lua_State *L)
{
    luaL_newlib(L, window_functions);
    return (1);
}

int window_addAndMakeVisible(lua_State *L)
{
    lua_settop(L, 1);

    SelectorComponent *component = *reinterpret_cast<SelectorComponent **>(
        luaL_checkudata(L, 1, "Component"));

    if (component) {
        System::windowManager.getActiveWindow()->addAndMakeVisible(component);
        System::windowManager.repaintActive();
    }

    return (0);
}

int window_repaint(lua_State *L)
{
    System::windowManager.repaintActive();
    return (0);
}

int window_clear(lua_State *L)
{
    //System::windowManager.getActiveWindow()->clear ();
    return (0);
}

int window_findChildById(lua_State *L)
{
    lua_settop(L, 1);
    int id = luaL_checkinteger(L, -1);

    SelectorComponent *component = reinterpret_cast<SelectorComponent *>(
        System::windowManager.getActiveWindow()->findChildById(id));

    if (component) {
        *reinterpret_cast<SelectorComponent **>(
            lua_newuserdata(L, sizeof(SelectorComponent *))) = component;

        luaL_getmetatable(L, "Component");
        lua_setmetatable(L, -2);

        return (1);
    }

    return (0);
}
