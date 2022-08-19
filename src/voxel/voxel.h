#ifndef VOXEL_H
#define VOXEL_H

#include <stdlib.h>

#include "../mesh/mesh.h"

struct Chunk {
  size_t x;
  size_t y;
  size_t z;
  size_t dimID;
  size_t* voxelIDs;
  struct ChunkMesh* mesh;
};

struct VoxelType {
  const char* name;
  size_t textureID;
};

void destroyChunk(struct Chunk*);

#endif
