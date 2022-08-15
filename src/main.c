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

void renderFront(float x, float y, float z) {
  rlNormal3f(0.0f, 0.0f, 1.0f);
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);  // Bottom Left Of The Texture and Quad
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);  // Bottom Right Of The Texture and Quad
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);  // Top Right Of The Texture and Quad
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);  // Top Left Of The Texture and Quad
}

void renderBack(float x, float y, float z) {
  rlNormal3f(0.0f, 0.0f, - 1.0f);                  // Normal Pointing Away From Viewer
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);  // Bottom Right Of The Texture and Quad
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);  // Top Right Of The Texture and Quad
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);  // Top Left Of The Texture and Quad
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);  // Bottom Left Of The Texture and Quad
}

void renderTop(float x, float y, float z) {
  rlNormal3f(0.0f, 1.0f, 0.0f);                  // Normal Pointing Up
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);  // Top Left Of The Texture and Quad
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);  // Bottom Left Of The Texture and Quad
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);  // Bottom Right Of The Texture and Quad
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);  // Top Right Of The Texture and Quad
}

void renderBottom(float x, float y, float z) {
  rlNormal3f(0.0f, - 1.0f, 0.0f);                  // Normal Pointing Down
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);  // Top Right Of The Texture and Quad
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);  // Top Left Of The Texture and Quad
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);  // Bottom Left Of The Texture and Quad
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);  // Bottom Right Of The Texture and Quad
}

void renderRight(float x, float y, float z) {
  rlNormal3f(1.0f, 0.0f, 0.0f);                  // Normal Pointing Right
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);  // Bottom Right Of The Texture and Quad
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);  // Top Right Of The Texture and Quad
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);  // Top Left Of The Texture and Quad
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);  // Bottom Left Of The Texture and Quad
}

void renderLeft(float x, float y, float z) {
  rlNormal3f( - 1.0f, 0.0f, 0.0f);                  // Normal Pointing Left
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);  // Bottom Left Of The Texture and Quad
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);  // Bottom Right Of The Texture and Quad
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);  // Top Right Of The Texture and Quad
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);  // Top Left Of The Texture and Quad
}

void renderMeshFace(Texture2D texture, Vector3 position, uint8_t side) {
  float x = position.x;
  float y = position.y;
  float z = position.z;

  rlCheckRenderBatchLimit(36);

  rlSetTexture(texture.id);

  rlBegin(RL_QUADS);
    rlColor4ub(0, 0, 0, 255);

    switch(side) {
      case 0: renderFront(x, y, z); break;
      case 1: renderBack(x, y, z); break;
      case 2: renderTop(x, y, z); break;
      case 3: renderBottom(x, y, z); break;
      case 4: renderRight(x, y, z); break;
      case 5: renderLeft(x, y, z); break;
    }
  rlEnd();

  rlSetTexture(0);
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
