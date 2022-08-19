/* mesh.c */

#include "mesh.h"

void initializeMesh(struct ChunkMesh* mesh) {
  mesh->cap = 8; // arbitrary value
  mesh->size = 0;
  mesh->faces = calloc(mesh->cap, sizeof(struct MeshFace));
}

void appendMeshFace(struct ChunkMesh* mesh, struct MeshFace face) {
  // Upon sufficient allocated space, simply add the new face
  if(mesh->size + 1 < mesh->cap) {
    mesh->faces[mesh->size++] = face;
    return;
  }

  mesh->cap *= 1.5;

  // Allocate new array for new faces
  struct MeshFace* newFaces = calloc(mesh->cap, sizeof(struct MeshFace));
  for(size_t i = 0; i < mesh->size; i++) { // Copy all current faces to new allocated array
    newFaces[i] = mesh->faces[i];
  }
  
  free(mesh->faces); // Free the old allocated array
  mesh->faces = newFaces; // Point to the new allocated array

  // Finally just add the new face
  mesh->faces[mesh->size++] = face;
}
