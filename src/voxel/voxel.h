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
  size_t chunkListCap;
  size_t chunkListSize;

  struct VoxelType* voxelTypes;
  size_t typeListSize;

  Texture2D* textures;
  size_t textureListSize;

  struct {
    size_t renderDistance;
    size_t chunkSize;
    char* modDirectory;
  } settings;
};

#endif
