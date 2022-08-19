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

size_t uintPow(size_t base, size_t exponent);

void initializeMesh(struct ChunkMesh*);
void appendMeshFace(struct ChunkMesh*, struct MeshFace);

#endif