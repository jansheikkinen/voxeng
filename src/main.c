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

struct Voxel* getAdjacentVoxel(
  struct Game game, struct Voxel voxel,
  int xOffset, int yOffset, int zOffset
) {
  return worldVector3ToVoxel(
    game.worldlist.worlds[0],
    (Vector3) {
      ((int)voxel.position.x + xOffset) / chunkSize,
      ((int)voxel.position.y + yOffset) / chunkSize,
      ((int)voxel.position.z + zOffset) / chunkSize
    },
    (Vector3) {
      ((int)voxel.position.x + xOffset) % chunkSize,
      ((int)voxel.position.y + yOffset) % chunkSize,
      ((int)voxel.position.z + zOffset) % chunkSize
    }
  );
}

void checkMeshFace(
  struct Game game, struct Chunk* chunk, struct Voxel voxel,
  int xOffset, int yOffset, int zOffset, uint8_t side
) {
  if(getAdjacentVoxel(game, voxel, xOffset, yOffset, zOffset)->id != 0) {
    appendMeshFace(chunk->mesh, (struct MeshFace){
      voxel.position.x, voxel.position.y, voxel.position.z, side
    });
  }
}

void reloadChunkMesh(struct Game game, struct Chunk* chunk) {
  game.mesh.size = 0;
  for(size_t i = 0; i < chunkSize; i++) {
    if(chunk->voxels[i].id == 0) continue;
    struct Voxel voxel = chunk->voxels[i];
    // Check if voxel in front has an id of zero
    checkMeshFace(game, chunk, voxel, 0,  0,  1, 0);
    // Check if voxel behind   has an id of zero
    checkMeshFace(game, chunk, voxel, 0,  0, -1, 1);
    // Check if voxel above    has an id of zero
    checkMeshFace(game, chunk, voxel, 0,  1,  0, 2);
    // Check if voxel below    has an id of zero
    checkMeshFace(game, chunk, voxel, 0, -1,  0, 3);
    // Check if voxel to right has an id of zero
    checkMeshFace(game, chunk, voxel, 1,  0,  0, 4);
    // Check if voxel to left  has an id of zero
    checkMeshFace(game, chunk, voxel, -1, 0,  0, 5);
  }
}

void renderChunk(struct Game game, struct Chunk* chunk) {
  const size_t offset = worldSize * chunkSize * voxelSize;
  const Color color = (Color){255, 255, 255, 255};
  const size_t lim = pow(chunkSize, 3);
  for(size_t i = 0; i < lim; i++) {
    if(chunk->voxels[i].id == 0) continue;
    DrawCubeTexture(
      game.voxelDataList.voxelData[chunk->voxels[i].id].texture,
      (Vector3){
        chunk->voxels[i].position.x - offset,
        chunk->voxels[i].position.y,
        chunk->voxels[i].position.z - offset
      },
      voxelSize, voxelSize, voxelSize,
      color
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
      renderChunk(game, game.worldlist.worlds[0]->chunks[i]);
    }

    // Draw grids to show voxel, chunk, and world sizes
    // DrawGrid(2 * chunkSize * voxelSize, voxelSize);
    DrawGrid(worldSize * chunkSize / 2, chunkSize);

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
