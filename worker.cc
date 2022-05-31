#include <emscripten.h>
// #include "geometry.h"
// #include "compose.h"
// #include "noise.h"
// #include "march.h"
// #include "collide.h"
#include "physics.h"
// #include "convex.h"
// #include "earcut.h"
// #include <iostream>
// #include "cut.h"
#include <deque>
#include <map>

extern "C" {

// memory

EMSCRIPTEN_KEEPALIVE void *doMalloc(size_t size) {
  return malloc(size);
}

EMSCRIPTEN_KEEPALIVE void doFree(void *ptr) {
  free(ptr);
}

//

EMSCRIPTEN_KEEPALIVE PBase *makePhysicsBase() {
  return new PBase();
}

EMSCRIPTEN_KEEPALIVE void cookGeometryPhysics(PBase *base, float *positions, unsigned int *indices, unsigned int numPositions, unsigned int numIndices, uint8_t **data, unsigned int *length, PxDefaultMemoryOutputStream **writeStream) {
  base->cookGeometry(positions, indices, numPositions, numIndices, data, length, writeStream);
}
EMSCRIPTEN_KEEPALIVE void cookConvexGeometryPhysics(PBase *base, float *positions, unsigned int *indices, unsigned int numPositions, unsigned int numIndices, uint8_t **data, unsigned int *length, PxDefaultMemoryOutputStream **writeStream) {
  base->cookConvexGeometry(positions, indices, numPositions, numIndices, data, length, writeStream);
}

EMSCRIPTEN_KEEPALIVE void deleteMemoryOutputStream(PxDefaultMemoryOutputStream *writeStream) {
  delete writeStream;
}

} // extern "C"