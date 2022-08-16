#ifndef MESH_H
#define MESH_H

#include <stdint.h>

#include <raylib.h>
#include <rlgl.h>

void renderFront(float, float, float);
void renderBack(float, float, float);
void renderTop(float, float, float);
void renderBottom(float, float, float);
void renderRight(float, float, float);
void renderLeft(float, float, float);

void renderMeshFace(Texture2D, Vector3, uint8_t);

#endif