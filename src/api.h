#ifndef API_H
#define API_H

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "voxel.h"

#include <dirent.h>

void initializeLua();

void doLuaFiles(lua_State*);

#endif