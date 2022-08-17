/* voxel.c */

#include "gamedata.h"

#include <stdio.h>

// INDEX FUNCTIONS

size_t uintPow(size_t base, size_t exponent) {
  size_t n = 1;
  for(register size_t i = 0; i < exponent; i++) {
    n *= base;
  }
  return n;
}

void initializeGameData(
  struct GameData* game, const size_t renderDistance,
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

void destroyGameData(struct GameData* game) {
  free(game->voxelTypes);
  free(game->textures);

  for(size_t i = 0; i < game->chunkListSize; i++) {
    destroyChunk(game->loadedChunks[i]);
  }
}

void loadChunk(struct GameData* game, size_t x, size_t y, size_t z, size_t dimID) {
  game->loadedChunks[game->chunkListSize] = malloc(sizeof(struct Chunk));
  game->loadedChunks[game->chunkListSize]->x = 0;
  game->loadedChunks[game->chunkListSize]->y = 1;
  game->loadedChunks[game->chunkListSize]->z = 2;
  game->loadedChunks[game->chunkListSize]->dimID = 2;
  game->loadedChunks[game->chunkListSize]->voxelIDs = calloc(uintPow(game->settings.chunkSize, 3), sizeof(size_t));
  game->chunkListSize++;
}
