#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <stdlib.h>
#include <stdint.h>

#include <raylib.h>

#include "../voxel/voxel.h"

struct GameData {
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
    const char* modDirectory;
  } settings;
};

void initializeGameData(struct GameData*, size_t, size_t, const char*);
void destroyGameData(struct GameData*);

#endif
