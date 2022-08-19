/* voxel.c */

#include "voxel.h"

#include <stdio.h>

size_t posToIndex(int x, int y, int z, int mult) {
  return abs((z * mult * mult) + (y * mult) + x);
}

void destroyChunk(struct Chunk* chunk) {
  free(chunk->voxelIDs);
  free(chunk);
}
