/* voxel.c */

#include "voxel.h"

#include <stdio.h>

// INDEX FUNCTIONS

size_t posToIndex(int x, int y, int z, int mult) {
  return abs((z * mult * mult) + (y * mult) + x);
}

size_t uintPow(size_t base, size_t exponent) {
  size_t n = 1;
  for(register size_t i = 0; i < exponent; i++) {
    n *= base;
  }
  return n;
}
