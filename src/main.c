/* main.c */

#include <stdlib.h>

#include <math.h>
#include <raylib.h>

// Screen Dimensions
// For some reason raylib bypasses i3 and makes the window floating regardless??
const size_t scrWidth = 500;
const size_t scrHeight = 500;

// The size of a voxel, number of voxels in a chunk,
// and number of chunks in the world
const size_t voxelSize = 2;
const size_t chunkSize = 4;
const size_t worldSize = 2;

// Voxel IDs
enum VoxelType {
  VOXEL_VOID  = 0,
  VOXEL_DIRT  = 1,
  VOXEL_STONE = 2,
};

// Just a placeholder
// Get the voxel's color by using its type(above) as the index
const Color voxelColors[] = {
  {   0,   0,   0, 0 },
  { 140,  80,  30, 255 },
  { 120, 120, 120, 255 },
};

// The basic building blocks
struct Voxel {
  Vector3 position;
  enum VoxelType type;
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


  // Game loop
  while(!WindowShouldClose()) {
    // Game logic
    UpdateCamera(&camera);


    // Start rendering
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // The 3D stuff
    BeginMode3D(camera);

    // World would be rendered here

    // Draw grids to show voxel, chunk, and world sizes
    DrawGrid(chunkSize * voxelSize, voxelSize);
    DrawGrid(worldSize * chunkSize, chunkSize);

    EndMode3D();

    // The 2D stuff
    DrawText("VoxEng Alpha", 10, 10, 20, BLACK);
    DrawFPS(10, 30);

    EndDrawing();
  }

  // Actually close the window
  CloseWindow();
  return 0;
}
