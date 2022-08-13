/* voxel.c */

#include "voxel.h"

#include <stdio.h>

// INDEX FUNCTIONS

size_t posToIndex(int x, int y, int z, int mult) {
  return abs((z * mult * mult) + (y * mult) + x);
}

size_t getChunkIndex(int x, int y, int z) {
  return posToIndex(x, y, z, worldSize);
}

size_t getVoxelIndex(int x, int y, int z) {
  return posToIndex(x, y, z, chunkSize);
}

struct Voxel* chunkPosToVoxel(struct Chunk* chunk, int x, int y, int z) {
  return &chunk->voxels[getVoxelIndex(x, y, z)];
}

struct Voxel* chunkVector3ToVoxel(struct Chunk* chunk, Vector3 position) {
  return &chunk->voxels[getVoxelIndex(position.x, position.y, position.z)];
}

struct Chunk* worldPosToChunk(struct World* world, int x, int y, int z) {
  return world->chunks[getChunkIndex(x, y, z)];
}

struct Chunk* worldVector3ToChunk(struct World* world, Vector3 position) {
  return world->chunks[getChunkIndex(position.x, position.y, position.z)];
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
  chunk->voxels = calloc(pow(chunkSize, 3), sizeof(struct Voxel));
  chunk->position = position;

  for(size_t z = 0; z < chunkSize; z++) {
    for(size_t y = 0; y < chunkSize; y++) {
      for(size_t x = 0; x < chunkSize; x++) {
        float voxelSizeHalf = voxelSize / 2.0;

        struct Voxel* voxel = chunkPosToVoxel(chunk, x, y, z);
        voxel->id = 0;
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
        size_t index = posToIndex(x, y, z, worldSize);
        world->chunks[index] = malloc(sizeof(struct Chunk));
        initializeChunk(world->chunks[index], (Vector3){ x, y, z });
      }
    }
  }
}

void initializeWorldList(struct WorldList* worldlist) {
  worldlist->capacity = 4;
  worldlist->size = 0;
  worldlist->worlds = calloc(worldlist->capacity, sizeof(struct World*));
}

void initializeVoxelDataList(struct VoxelDataList* vdlist) {
  vdlist->capacity = 4;
  vdlist->size = 0;
  vdlist->voxelData = calloc(vdlist->capacity, sizeof(struct VoxelData));
  appendVoxelData(vdlist, (struct VoxelData){"null_voxel", (Texture2D){0}});
}

void initializeGame(struct Game* game) {
  initializeWorldList(&game->worldlist);
  initializeVoxelDataList(&game->voxelDataList);
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

void destroyVoxelDataList(struct VoxelDataList* voxelDataList) {
  free(voxelDataList->voxelData);
}

void destroyWorldList(struct WorldList* worldlist) {
  for(size_t i = 0; i < worldlist->size; i++) {
    destroyWorld(worldlist->worlds[i]);
  }
}

void destroyGame(struct Game* game) {
  destroyWorldList(&game->worldlist);
}

// WORLD LIST APPENDING

void appendVoxelData(struct VoxelDataList* vdlist, struct VoxelData voxelData) {
  if(vdlist->size + 1 >= vdlist->capacity) {
    vdlist->capacity *= 1.5;

    struct VoxelData* newVdlist = calloc(vdlist->capacity, sizeof(struct VoxelData));

    for(size_t i = 0; i < vdlist->size; i++) {
      newVdlist[i] = vdlist->voxelData[i];
    }

    vdlist->voxelData = newVdlist;
    vdlist->voxelData[vdlist->size++] = voxelData;
  } else {
    vdlist->voxelData[vdlist->size++] = voxelData;
  }
}

void appendWorldList(struct WorldList* worldlist, struct World* world) {
  printf("%ld\n", (long)worldlist);

  if(worldlist->size + 1 >= worldlist->capacity) {
    worldlist->capacity *= 1.5;
    
    struct World** newWorldlist = calloc(worldlist->capacity, sizeof(struct World*));
    
    for(size_t i = 0; i < worldlist->size; i++) {
      newWorldlist[i] = worldlist->worlds[i];
    }
    
    worldlist->worlds = newWorldlist;
    worldlist->worlds[worldlist->size++] = world;
  } else {
    worldlist->worlds[worldlist->size++] = world;
    printf("%ld\n", (long)worldlist->worlds[worldlist->size - 1]);
    printf("%ld\n", (long)worldlist->worlds[worldlist->size]);
  }
}
