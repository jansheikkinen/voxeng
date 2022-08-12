/* main.c */

#include <stdlib.h>

#include <math.h>
#include <raylib.h>

#include "voxel/voxel.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// Screen Dimensions
// For some reason raylib bypasses i3 and makes the window floating regardless??
const size_t scrWidth = 500;
const size_t scrHeight = 500;

// Placehold voxel colours
const Color voxelColors[] = {
  {   0,   0,   0,   0 },
  { 140,  80,  30, 255 },
  { 120, 120, 120, 255 },
};

void renderVoxel(struct Voxel voxel) {
  DrawCube(
      voxel.position,
      voxelSize, voxelSize, voxelSize,
      voxelColors[voxel.type]);

  DrawCubeWires(
      voxel.position,
      voxelSize, voxelSize, voxelSize,
      BLACK);
}

int l_setVoxel(lua_State *L) {
  const float x = lua_tonumber(L, -3);
  const float y = lua_tonumber(L, -2);
  const float z = lua_tonumber(L, -1);
  
  lua_getglobal(L, "_World");
  struct World *world = lua_touserdata(L, -1);

  worldVector3ToVoxel(world, (Vector3){(int)(x)/chunkSize, (int)(y)/chunkSize, (int)(z)/chunkSize}, (Vector3){(int)(x)%chunkSize, (int)(y)%chunkSize, (int)(z)%chunkSize})->type = VOXEL_UNDEFINED;

  return 0;
}

int main(void) {
  // Initialise the window
  InitWindow(scrWidth, scrHeight, "test");

  struct World world = { 0 };
  initializeWorld(&world);

  // Initialize Lua state
  lua_State *L = luaL_newstate();

  // Load Lua libraries
  luaL_openlibs(L); 

  // Load world global into Lua
  lua_pushlightuserdata(L, &world);
  lua_setglobal(L, "_World");

  // Load C functions into Lua
  lua_createtable(L, 0, 4);

  lua_pushstring(L, "setVoxel");
  lua_pushcfunction(L, l_setVoxel);
  lua_settable(L, -3);

  lua_setglobal(L, "voxeng");

  // Do file
  luaL_dofile(L, "src/script.lua");

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
        renderVoxel(world.chunks[i]->voxels[j]);
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

  destroyWorld(&world);
  return 0;
}
