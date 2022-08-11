/* voxel.c */

#include "voxel.h"

#include <stdio.h>
// INDEX FUNCTIONS

size_t posToIndex(int x, int y, int z, int mult) {
  return abs((z * mult * mult) + (y * mult) + x);
}

struct Voxel* chunkPosToVoxel(struct Chunk* chunk, int x, int y, int z) {
  return &chunk->voxels[posToIndex(x, y, z, chunkSize)];
}

struct Voxel* chunkVector3ToVoxel(struct Chunk* chunk, Vector3 position) {
  return &chunk->voxels[
    posToIndex(position.x, position.y, position.z, chunkSize)];
}

struct Chunk* worldPosToChunk(struct World* world, int x, int y, int z) {
  return world->chunks[posToIndex(x, y, z, worldSize)];
}

struct Chunk* worldVector3ToChunk(struct World* world, Vector3 position) {
  return world->chunks[
    posToIndex(position.x, position.y, position.z, worldSize)];
}

struct Voxel* worldPosToVoxel(struct World* world,
    int worldx, int worldy, int worldz,
    int chunkx, int chunky, int chunkz) {
  return chunkPosToVoxel(
      worldPosToChunk(world, worldx, worldy, worldz),
      chunkx, chunky, chunkz);
}

struct Voxel* worldVector3ToVoxel(struct World* world,
    Vector3 worldPos, Vector3 chunkPos) {
  return chunkVector3ToVoxel(worldVector3ToChunk(world, worldPos), chunkPos);
}



// INITIALIZATION FUNCTIONS

void initializeChunk(struct Chunk* chunk, Vector3 position) {
  chunk->voxels = calloc(pow(chunkSize, 3), sizeof(*chunk->voxels));
  chunk->position = position;

  for(size_t z = 0; z < chunkSize; z++) {
    for(size_t y = 0; y < chunkSize; y++) {
      for(size_t x = 0; x < chunkSize; x++) {
        float voxelSizeHalf = voxelSize / 2.0;

        struct Voxel* voxel = chunkPosToVoxel(chunk, x, y, z);
        voxel->type = VOXEL_STONE;
        voxel->position = (Vector3){
          (chunkSize * position.x) + (x * voxelSize) + voxelSizeHalf,
          (chunkSize * position.y) + (y * voxelSize) + voxelSizeHalf,
          (chunkSize * position.z) + (z * voxelSize) + voxelSizeHalf,
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
        struct Chunk* chunk = worldPosToChunk(world, x, y, z);
        chunk = malloc(sizeof(struct Chunk));

        initializeChunk(chunk, (Vector3){ x, y, z });
      }
    }
  }
}



// DESTRUCTOR FUNCTIONS

void destroyChunk(struct Chunk* chunk) {
  free(chunk->voxels);
}

void destroyWorld(struct World* world) {
  for(size_t z = 0; z < worldSize; z++) {
    for(size_t y = 0; y < worldSize; y++) {
      for(size_t x = 0; x < worldSize; x++) {
        destroyChunk(worldPosToChunk(world, x, y, z));
      }
    }
  }
}
