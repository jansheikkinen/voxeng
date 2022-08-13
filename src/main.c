/* main.c */

#include <stdlib.h>

#include <math.h>
#include <raylib.h>

#include "voxel/voxel.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <dirent.h>

// Screen Dimensions
// For some reason raylib bypasses i3 and makes the window floating regardless??
const size_t scrWidth = 1000;
const size_t scrHeight = 600;

void renderVoxel(struct Game game, struct Voxel voxel) {
  if(voxel.id != 0) {
    DrawCubeTexture(
      game.voxelDataList.voxelData[voxel.id].texture,
      voxel.position,
      voxelSize, voxelSize, voxelSize,
      (Color){255, 255, 255, 255}
    );
  }
}

int l_setVoxel(lua_State *L) {
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

int l_regVoxel(lua_State *L) {
  const char* name = lua_tostring(L, -2);
  const char* texture = lua_tostring(L, -1);

  lua_getglobal(L, "_game_data");
  struct Game* game = lua_touserdata(L, -1);

  appendVoxelData(&game->voxelDataList, (struct VoxelData){name, LoadTexture(texture)});
  printf("%s\n", game->voxelDataList.voxelData[1].name);

  lua_pushinteger(L, game->voxelDataList.size - 1);

  return 1;
}

int main(void) {
  // Initialise the window
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(scrWidth, scrHeight, "test");
  MaximizeWindow();

  struct Game game = { 0 };
  initializeGame(&game);

  struct World world = { 0 };
  initializeWorld(&world);

  appendWorldList(&game.worldlist, &world);

  // Initialize Lua state
  lua_State *L = luaL_newstate();

  // Load Lua libraries
  luaL_openlibs(L); 

  // Load world global into Lua
  lua_pushlightuserdata(L, &game);
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

  // Do file
  // luaL_dofile(L, "build/mods/script.lua");

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

  // Initialise the camera
  Camera3D camera = { 0 };
  camera.position = (Vector3){ chunkSize, chunkSize, chunkSize };
  camera.target = (Vector3){ 0, 0, 0 };
  camera.up = (Vector3){ 0, 1, 0 };
  camera.fovy = 80;
  camera.projection = CAMERA_PERSPECTIVE;
  SetCameraMode(camera, CAMERA_FREE);
  // You can pan with middle mouse
  // and rotate by holding alt and doing middle mouse

  // Game loop
  while(!WindowShouldClose()) {
    // Game logic
    UpdateCamera(&camera);


    // Start rendering
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // The 3D stuff
    BeginMode3D(camera);

    // Render the world
    for(size_t i = 0; i < pow(worldSize, 3); i++) {
      for(size_t j = 0; j < pow(chunkSize, 3); j++) {
        renderVoxel(game, game.worldlist.worlds[0]->chunks[i]->voxels[j]);
      }
    }

    // Draw grids to show voxel, chunk, and world sizes
    // DrawGrid(2 * chunkSize * voxelSize, voxelSize);
    DrawGrid(worldSize * chunkSize, chunkSize);

    EndMode3D();

    // The 2D stuff
    DrawText("VoxEng Alpha", 10, 10, 20, BLACK);
    DrawFPS(10, 30);

    EndDrawing();
  }

  // Actually close the window
  CloseWindow();

  destroyGame(&game);
  return 0;
}
