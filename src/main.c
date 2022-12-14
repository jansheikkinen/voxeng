/* main.c */

#include <stdlib.h>
#include <dirent.h>

#include <math.h>
#include <raylib.h>
#include <rlgl.h>

#include "gamedata/gamedata.h"


// Screen Dimensions
const size_t scrWidth = 1000;
const size_t scrHeight = 600;

int main(void) {
  // Initialise the window
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(scrWidth, scrHeight, "test");
  MaximizeWindow();

  const int chunkSize = 4;
  const int renderDistance = 16;
  struct GameData gamedata;
  initializeGameData(&gamedata, renderDistance, chunkSize, "build/mods");

  // Initialize Lua state
  //lua_State* L = initializeLua(&game);

  //doLuaFiles(L);

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

    // Refresh game
    /*if(IsKeyPressed(KEY_F1)) {
      // Unload stored textures
      for(size_t i = 0; i < game.voxelDataList.size; i++) {
        UnloadTexture(game.voxelDataList.voxelData[i].texture);
      }
      game.voxelDataList.size = 1;

      doLuaFiles(L);
    }*/

    // Start rendaering
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // The 3D stuff
    BeginMode3D(camera);

    // Render the world

    // Draw grids to show voxel, chunk, and world sizes
    // DrawGrid(2 * chunkSize * voxelSize, voxelSize);
    DrawGrid(renderDistance * chunkSize / 2, chunkSize);

    EndMode3D();

    // The 2D stuff
    DrawText("VoxEng Alpha", 10, 10, 20, BLACK);
    DrawFPS(10, 30);

    EndDrawing();
  }

  // Actually close the window
  CloseWindow();

  destroyGameData(&gamedata);
  return 0;
}
