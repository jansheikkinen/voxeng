/* voxel.c */

#include "gamedata.h"

#include <stdio.h>

// INDEX FUNCTIONS

size_t posToIndex(int x, int y, int z, int mult) {
  return abs((z * mult * mult) + (y * mult) + x);
}

size_t uintPow(size_t base, size_t exponent) {
  size_t n = 1;
  for(register size_t i = 0; i < exponent; i++) {
    n *= base;
  }
  return n;
}

void initializeGame(
  struct Game* game, const size_t renderDistance,
  const size_t chunkSize, const char* modDirectory
) {
  game->settings.renderDistance = renderDistance;
  game->settings.chunkSize = chunkSize;
  game->settings.modDirectory = modDirectory;

  game->chunkListSize = 0;
  game->chunkListCap = uintPow(renderDistance, 3);
  game->loadedChunks = calloc(game->chunkListCap, sizeof(struct Chunk*));

  game->typeListSize = 1;
  game->voxelTypes = calloc(1, sizeof(struct VoxelType));

  game->textureListSize = 1;
  game->textures = calloc(1, sizeof(Texture2D));
}
