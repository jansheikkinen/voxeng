/* main.c */

#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include <raylib.h>

// Screen Dimensions
// For some reason raylib bypasses i3 and makes the window floating regardless??
const size_t scrWidth = 500;
const size_t scrHeight = 500;

// The size of a voxel, number of voxels in a chunk,
// and number of chunks in the world
const size_t voxelSize = 1; // TODO: !1 sizes still kinda funky
const size_t chunkSize = 4;
const size_t worldSize = 2;

// Voxel IDs
enum VoxelType {
  VOXEL_VOID  = 0,
  VOXEL_DIRT  = 1,
  VOXEL_STONE = 2,
};

// The basic building blocks
struct Voxel {
  Vector3 position;
  enum VoxelType type;
  Texture2D texture;
};

// Groups of voxels
struct Chunk {
  Vector3 position;
  struct Voxel* voxels;
};

// Groups of chunks
struct World {
  struct Chunk** chunks;
};

size_t posToIndex(Vector3 pos, int mult) {
  return (pos.z * mult * mult) + (pos.y * mult) + pos.x;
}

void initializeChunk(struct Chunk* chunk, Vector3 position) {
  chunk->voxels = calloc(pow(chunkSize, 3), sizeof(*chunk->voxels));
  chunk->position = position;

  for(size_t z = 0; z < chunkSize; z++) {
    for(size_t y = 0; y < chunkSize; y++) {
      for(size_t x = 0; x < chunkSize; x++) {
        float voxelSizeHalf = voxelSize / 2.0;

        chunk->voxels[posToIndex((Vector3){ x, y, z }, chunkSize)] =
          (struct Voxel){
            (Vector3){
              (chunkSize * position.x) + (x * voxelSize) + voxelSizeHalf,
              (chunkSize * position.y) + (y * voxelSize) + voxelSizeHalf,
              (chunkSize * position.z) + (z * voxelSize) + voxelSizeHalf,
            },
           VOXEL_STONE,
           LoadTexture("texture.png")
        };
      }
    }
  }
}

void initializeWorld(struct World* world) {
  world->chunks = calloc(pow(worldSize, 3), sizeof(struct Chunk*));

  for(size_t z = 0; z < worldSize; z++) {
    for(size_t y = 0; y < worldSize; y++) {
      for(size_t x = 0; x < worldSize; x++) {
        Vector3 pos = { x, y, z };

        world->chunks[posToIndex(pos, worldSize)] =
          malloc(sizeof(struct Chunk));

        initializeChunk(world->chunks[posToIndex(pos, worldSize)], pos);
      }
    }
  }
}

void destroyChunk(struct Chunk* chunk) {
  free(chunk->voxels);
}

void destroyWorld(struct World* world) {
  for(size_t z = 0; z < worldSize; z++) {
    for(size_t y = 0; y < worldSize; y++) {
      for(size_t x = 0; x < worldSize; x++) {
        destroyChunk(world->chunks[posToIndex(
              (Vector3){ x, y, z },
              worldSize)]);
      }
    }
  }
}

void renderVoxel(struct Voxel voxel) {
  DrawCubeTexture(
      voxel.texture,
      voxel.position,
      voxelSize, voxelSize, voxelSize,
      (Color){255, 255, 255, 255});

  // DrawCubeWires(
  //     voxel.position,
  //     voxelSize, voxelSize, voxelSize,
  //     BLACK);
}


int main(void) {
  // Initialise the window
  InitWindow(scrWidth, scrHeight, "test");

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

  struct World world = { 0 };
  initializeWorld(&world);

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
    DrawGrid(2 * chunkSize * voxelSize, voxelSize);
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
