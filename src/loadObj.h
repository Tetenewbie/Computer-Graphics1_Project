#ifndef loadobj_c_guard
#define loadobj_c_guard 1

#include <stddef.h>

typedef float Vec2[2];
typedef float Vec3[3];

typedef struct Vertex {
  Vec3 v;
  Vec2 vt;
  Vec3 vn;
} Vertex;

typedef Vertex Triangle[3];

/**
 * Loads vertex data from an OBJ file.
 * 
 * This is just a simple demonstration. Wavefront OBJ is not fully supported!
 * Use with triangulated meshes, only!
 * 
 * This function returns an array with the vertex data:
 * 3 floats for position
 * 2 floats for texture coordinates
 * 3 floats for normal vector
 * 
 * @param location - name of the OBJ file
 * @param vertexCount - number of vertices is written to this variable
 * @returns array with vertex data (has to be freed by caller)
 */
float* loadObj(char* location, size_t* vertexCount);

#endif