/* mesh.c */

#include "mesh.h"

void renderFront(float x, float y, float z) {
  rlNormal3f(0.0f, 0.0f, 1.0f);
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);  // Bottom Left Of The Texture and Quad
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);  // Bottom Right Of The Texture and Quad
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);  // Top Right Of The Texture and Quad
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);  // Top Left Of The Texture and Quad
}

void renderBack(float x, float y, float z) {
  rlNormal3f(0.0f, 0.0f, - 1.0f);                  // Normal Pointing Away From Viewer
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);  // Bottom Right Of The Texture and Quad
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);  // Top Right Of The Texture and Quad
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);  // Top Left Of The Texture and Quad
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);  // Bottom Left Of The Texture and Quad
}

void renderTop(float x, float y, float z) {
  rlNormal3f(0.0f, 1.0f, 0.0f);                  // Normal Pointing Up
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);  // Top Left Of The Texture and Quad
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);  // Bottom Left Of The Texture and Quad
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);  // Bottom Right Of The Texture and Quad
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);  // Top Right Of The Texture and Quad
}

void renderBottom(float x, float y, float z) {
  rlNormal3f(0.0f, - 1.0f, 0.0f);                  // Normal Pointing Down
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);  // Top Right Of The Texture and Quad
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);  // Top Left Of The Texture and Quad
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);  // Bottom Left Of The Texture and Quad
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);  // Bottom Right Of The Texture and Quad
}

void renderRight(float x, float y, float z) {
  rlNormal3f(1.0f, 0.0f, 0.0f);                  // Normal Pointing Right
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z - 0.5f);  // Bottom Right Of The Texture and Quad
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z - 0.5f);  // Top Right Of The Texture and Quad
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x + 0.5f, y + 0.5f, z + 0.5f);  // Top Left Of The Texture and Quad
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x + 0.5f, y - 0.5f, z + 0.5f);  // Bottom Left Of The Texture and Quad
}

void renderLeft(float x, float y, float z) {
  rlNormal3f( - 1.0f, 0.0f, 0.0f);                  // Normal Pointing Left
  rlTexCoord2f(0.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z - 0.5f);  // Bottom Left Of The Texture and Quad
  rlTexCoord2f(1.0f, 0.0f); rlVertex3f(x - 0.5f, y - 0.5f, z + 0.5f);  // Bottom Right Of The Texture and Quad
  rlTexCoord2f(1.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z + 0.5f);  // Top Right Of The Texture and Quad
  rlTexCoord2f(0.0f, 1.0f); rlVertex3f(x - 0.5f, y + 0.5f, z - 0.5f);  // Top Left Of The Texture and Quad
}

void renderMeshFace(Texture2D texture, Vector3 position, uint8_t side) {
  float x = position.x;
  float y = position.y;
  float z = position.z;

  rlCheckRenderBatchLimit(36);

  rlSetTexture(texture.id);

  rlBegin(RL_QUADS);
    rlColor4ub(0, 0, 0, 255);

    switch(side) {
      case 0: renderFront(x, y, z); break;
      case 1: renderBack(x, y, z); break;
      case 2: renderTop(x, y, z); break;
      case 3: renderBottom(x, y, z); break;
      case 4: renderRight(x, y, z); break;
      case 5: renderLeft(x, y, z); break;
    }
  rlEnd();

  rlSetTexture(0);
}