/* voxel.c */

#include "voxel.h"

#include <stdio.h>

// INDEX FUNCTIONS

size_t posToIndex(int x, int y, int z, int mult) {
  return abs((z * mult * mult) + (y * mult) + x);
}
