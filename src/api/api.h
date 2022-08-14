#ifndef API_H
#define API_H

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "../voxel/voxel.h"

#include <dirent.h>

lua_State* initializeLua(struct Game*);

void doLuaFiles(lua_State*);

#endif