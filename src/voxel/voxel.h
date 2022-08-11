#ifndef VOXEL_H
#define VOXEL_H

#include <stdlib.h>

#include <math.h>
#include <raylib.h>

#define voxelSize 1
#define chunkSize 4
#define worldSize 2

enum VoxelType {
  VOXEL_VOID,
  VOXEL_UNDEFINED,
  VOXEL_STONE,
};

struct Voxel {
  Vector3 position;
  enum VoxelType type;
};

struct Chunk {
  Vector3 position;
  struct Voxel* voxels;
};

struct World {
  struct Chunk** chunks;
};

struct Voxel* chunkPosToVoxel(struct Chunk*, int, int, int);
struct Voxel* chunkVector3ToVoxel(struct Chunk*, Vector3);

struct Chunk* worldPosToChunk(struct World*, int, int, int);
struct Chunk* worldVector3ToChunk(struct World*, Vector3);

struct Voxel* worldPosToVoxel(struct World*, int, int, int, int, int, int);
struct Voxel* worldVector3ToVoxel(struct World*, Vector3, Vector3);

void initializeChunk(struct Chunk*, Vector3);
void initializeWorld(struct World*);

void destroyChunk(struct Chunk*);
void destroyWorld(struct World*);

#endif
