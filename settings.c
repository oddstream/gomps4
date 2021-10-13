/* settings.c */

#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include <raylib.h>

#include "moon.h"

extern float cardScale;
extern Color baizeColor;

void LoadSettings(int *windowWidth, int *windowHeight)
{
    lua_State *L = luaL_newstate();

    if ( luaL_loadfile(L, "csol.settings.lua") || lua_pcall(L, 0, 0, 0) ) {
        fprintf(stderr, "ERROR: %s: col.settings.lua: %s\n", __func__, lua_tostring(L, -1));
        lua_pop(L, 1);
    } else {
        *windowWidth = MoonGetGlobalInt(L, "WindowWidth", 640);
        *windowHeight = MoonGetGlobalInt(L, "WindowHeight", 480);
        cardScale = MoonGetGlobalFloat(L, "CardScale", 1.0f);
        int typ = lua_getglobal(L, "BaizeColor");
        if ( typ == LUA_TTABLE ) {
            float r, g, b, a;
            r = MoonGetFieldFloat(L, "red", 1);
            g = MoonGetFieldFloat(L, "green", 1);
            b = MoonGetFieldFloat(L, "blue", 1);
            a = MoonGetFieldFloat(L, "alpha", 1);
            baizeColor = (Color){.r=r*255, .g=g*255, .b=b*255, .a=a*255};
        } else {
            baizeColor = DARKGREEN;
        }
        lua_pop(L, 1);  // remove global (table)
    }

    lua_close(L);
}

void SaveSettings(void)
{

}
