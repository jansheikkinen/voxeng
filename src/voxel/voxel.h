#ifndef VOXEL_H
#define VOXEL_H

#include <stdlib.h>
#include <stdint.h>

#include <math.h>
#include <raylib.h>

struct VoxelType {
  const char* name;
  size_t textureID;
};

struct Chunk {
  size_t x;
  size_t y;
  size_t z;
  size_t dimID;
  size_t* voxelIDs;
};

struct Game {
  struct Chunk** loadedChunks;
  size_t chunkCapacity;
  size_t chunkSize;

  struct VoxelType* voxelType;
  size_t typeSize;

  Texture2D* textures;
  size_t textureSize;

  struct {
    size_t renderDistance;
    size_t chunkSize;
    char* modDirectory;
  } settings;
};

#endif
