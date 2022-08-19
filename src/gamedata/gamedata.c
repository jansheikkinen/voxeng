/* voxel.c */

#include "gamedata.h"

#include <stdio.h>

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
  game->loadedChunks[game->chunkListSize]->x = x;
  game->loadedChunks[game->chunkListSize]->y = y;
  game->loadedChunks[game->chunkListSize]->z = z;
  game->loadedChunks[game->chunkListSize]->dimID = dimID;
  game->loadedChunks[game->chunkListSize]->voxelIDs = calloc(uintPow(game->settings.chunkSize, 3), sizeof(size_t));
  game->chunkListSize++;
}

void unloadChunk(struct GameData* game, size_t x, size_t y, size_t z, size_t dimID) {
  register size_t index = 0;
  for(register size_t i = 0; i < game->chunkListSize; i++) {
    if(game->loadedChunks[i]->x == x && game->loadedChunks[i]->y == y
      && game->loadedChunks[i]->z == z && game->loadedChunks[i]->dimID == dimID
    ) {
      index = i;
    }
  }
  
  game->loadedChunks[index] = game->loadedChunks[game->chunkListSize - 1];
  free(game->loadedChunks[game->chunkListSize - 1]);
  game->chunkListSize--;
}
