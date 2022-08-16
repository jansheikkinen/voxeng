#ifndef VOXEL_H
#define VOXEL_H

struct Chunk {
  size_t x;
  size_t y;
  size_t z;
  size_t dimID;
  size_t* voxelIDs;
};

struct VoxelType {
  const char* name;
  size_t textureID;
};

#endif
