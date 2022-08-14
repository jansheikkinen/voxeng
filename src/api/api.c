/* api.c */

#include "api.h"

#include <stdio.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <dirent.h>

// API Functions

int l_setVoxel(lua_State* L) {
  const float x = lua_tonumber(L, -4);
  const float y = lua_tonumber(L, -3);
  const float z = lua_tonumber(L, -2);
  const size_t id = lua_tonumber(L, -1);
  
  lua_getglobal(L, "_game_data");
  struct Game* game = lua_touserdata(L, -1);

  worldVector3ToVoxel(
    game->worldlist.worlds[0],
    (Vector3) {
      (int)x / chunkSize,
      (int)y / chunkSize,
      (int)z / chunkSize
    },
    (Vector3) {
      (int)x % chunkSize,
      (int)y % chunkSize,
      (int)z % chunkSize
    })->id = id;

  return 0;
}

int l_regVoxel(lua_State* L) {
  const char* name = lua_tostring(L, -2);
  const char* texture = lua_tostring(L, -1);

  lua_getglobal(L, "_game_data");
  struct Game* game = lua_touserdata(L, -1);

  appendVoxelData(&game->voxelDataList, (struct VoxelData){name, LoadTexture(texture)});

  lua_pushinteger(L, game->voxelDataList.size - 1);

  return 1;
}


lua_State* initializeLua(struct Game* game) {
  // Initialize Lua state
  lua_State* L = luaL_newstate();

  // Load Lua libraries
  luaL_openlibs(L); 

  // Load world global into Lua
  lua_pushlightuserdata(L, game);
  lua_setglobal(L, "_game_data");

  // Load C functions into Lua
  lua_createtable(L, 0, 4);

  lua_pushstring(L, "setVoxel");
  lua_pushcfunction(L, l_setVoxel);
  lua_settable(L, -3);

  lua_pushstring(L, "regVoxel");
  lua_pushcfunction(L, l_regVoxel);
  lua_settable(L, -3);

  lua_setglobal(L, "voxeng");

  return L;
}

void doLuaFiles(lua_State* L) {
  struct dirent* entry;
  DIR* dp = opendir(modDir);

  if(dp != NULL) {
    readdir(dp); // Skip directories . and ..
    readdir(dp);
    while((entry = readdir(dp)) != NULL) {
      char s1[50] = modDir;
      char s3[] = "/main.lua";

      size_t length = 0;
      while(s1[length] != '\0') {
        length++;
      }
      for (size_t i = 0; entry->d_name[i] != '\0'; i++, length++) {
        s1[length] = entry->d_name[i];
      }
      for (size_t i = 0; s3[i] != '\0'; i++, length++) {
        s1[length] = s3[i];
      }
      s1[length] = '\0';
      luaL_dofile(L, s1);
    }
    closedir(dp);
  }
}
