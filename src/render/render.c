/* render.c */

#include "render.h"

#include <stdio.h>

void renderFace(struct GameData* game, struct MeshFace* face) {
  register size_t x = face->x;
  register size_t y = face->y;
  register size_t z = face->z;

  rlCheckRenderBatchLimit(36);

  rlSetTexture(game->textures[face->id].id);

  rlBegin(RL_QUADS);
    rlColor4ub(0, 0, 0, 255);
    switch(face->face) {
      case 0:
        rlNormal3f(0.0f, 0.0f, 1.0f);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
        break;
      
      case 1:
        rlNormal3f(0.0f, 0.0f, - 1.0f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
        break;
      
      case 2:
        rlNormal3f(0.0f, 1.0f, 0.0f);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
        break;

      case 3:
        rlNormal3f(0.0f, - 1.0f, 0.0f);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
        break;

      case 4:
        rlNormal3f(1.0f, 0.0f, 0.0f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);
        break;

      case 5:
        rlNormal3f( - 1.0f, 0.0f, 0.0f);
        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);
        break;
    }
  rlEnd();

  rlSetTexture(0);
}

void renderChunk(struct GameData* game, struct Chunk* chunk) {
  const size_t chunkSize = game->settings.chunkSize;
  const size_t numberOfVoxels = uintPow(chunkSize, 3);

  register size_t x = 0;
  register size_t y = 0;
  register size_t z = 0;

  for(size_t i = 0; i < numberOfVoxels; i++) {
    // Increment index position
    x += 1;
    if(x == chunkSize) { x = 0; y += 1; }
    if(y == chunkSize) { y = 0; z += 1; }

    printf("test");
    if(chunk->voxelIDs[i] == 0) continue; // Skip void

    DrawCubeTexture(
      game->textures[game->voxelTypes[chunk->voxelIDs[i]].textureID],
      (Vector3){x, y, z},
      1.0f, 1.0f, 1.0f,
      (Color){0, 0, 0, 255}
    );
  }
}
