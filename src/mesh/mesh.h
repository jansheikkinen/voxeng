#ifndef MESH_H
#define MESH_H

#include <stdlib.h>
#include <stdint.h>

struct MeshFace {
  size_t x;
  size_t y;
  size_t z;
  size_t id;
  uint8_t face;
};

struct ChunkMesh {
  struct MeshFace* faces;
  size_t cap;
  size_t size;
};

void initializeMesh(struct ChunkMesh*);

#endif