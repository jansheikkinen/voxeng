/* main.c */

#include <stdlib.h>

#include <math.h>
#include <raylib.h>

#include "voxel/voxel.h"
#include "api/api.h"

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
  lua_State* L = initializeLua(&game);

  doLuaFiles(L);

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

  printf("test\n\n");
  // Game loop
  while(!WindowShouldClose()) {
    // Game logic
    UpdateCamera(&camera);

    // Refresh game
    if(IsKeyPressed(KEY_F1)) {
      // Unload stored textures
      for(size_t i = 0; i < game.voxelDataList.size; i++) {
        UnloadTexture(game.voxelDataList.voxelData[i].texture);
      }
      game.voxelDataList.size = 1;

      doLuaFiles(L);
    }

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
