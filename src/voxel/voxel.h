#ifndef VOXEL_H
#define VOXEL_H

#include <stdlib.h>
#include <stdint.h>

#include <math.h>
#include <raylib.h>

struct VoxelType {
  const char* name;
  size_t textureID;
};

struct Chunk {
  size_t x;
  size_t y;
  size_t z;
  size_t dimID;
  size_t* voxelIDs;
};

struct Game {
  struct Chunk** loadedChunks;
  struct VoxelType* voxelType;
  Texture2D* textures;
  
  struct {
    size_t renderDistance;
    size_t chunkSize;
    char* modDirectory;
  } settings;
};

struct Voxel* chunkPosToVoxel(struct Chunk*, int, int, int);
struct Voxel* chunkVector3ToVoxel(struct Chunk*, Vector3);

struct Chunk* worldPosToChunk(struct World*, int, int, int);
struct Chunk* worldVector3ToChunk(struct World*, Vector3);

struct Voxel* worldPosToVoxel(struct World*, int, int, int, int, int, int);
struct Voxel* worldVector3ToVoxel(struct World*, Vector3, Vector3);

void initializeChunk(struct Chunk*, Vector3);
void initializeWorld(struct World*);
void initializeWorldList(struct WorldList*);
void initializeVoxelDataList(struct VoxelDataList*);
void initializeMesh(struct ChunkMesh*);
void initializeGame(struct Game*);

void destroyChunk(struct Chunk*);
void destroyWorld(struct World*);
void destroyWorldList(struct WorldList*);
void destroyVoxelDataList(struct VoxelDataList*);
void destroyMesh(struct ChunkMesh*);
void destroyGame(struct Game*);

void appendVoxelData(struct VoxelDataList*, struct VoxelData);
void appendWorldList(struct WorldList*, struct World* world);
void appendMeshFace(struct ChunkMesh*, struct MeshFace);

#endif
