#include "loadObj.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


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
float* loadObj(char* location, size_t* vertexCount) {
	
  FILE* file = fopen(location, "r");
  if (file == NULL) return NULL;

  ssize_t read = 0;

  // line buffer for getline
  size_t lineLength = 1024;
  char* line = malloc(lineLength);

  // count number of lines
  size_t vCount = 0;
  size_t vtCount = 0;
  size_t vnCount = 0;
  size_t fCount = 0;
  while ((read = getline(&line, &lineLength, file)) != -1) {
    if (strncmp(line, "v ", 2) == 0) ++vCount;
    else if (strncmp(line, "vt ", 3) == 0) ++vtCount;
    else if (strncmp(line, "vn ", 3) == 0) ++vnCount;
    else if (strncmp(line, "f ", 2) == 0) ++fCount;
  }

  // allocate temporary buffers
  Vec3* v = malloc(vCount * sizeof(Vec3));
  Vec2* vt = malloc(vtCount * sizeof(Vec2));
  Vec3* vn = malloc(vnCount * sizeof(Vec3));

  // allocate output buffer
  Triangle* buffer = malloc(fCount * sizeof(Triangle));

  vCount = 0;
  vtCount = 0;
  vnCount = 0;
  fCount = 0;

  // jump to start of file
  fseek(file, 0, SEEK_SET);
  
  // process each line
  while ((read = getline(&line, &lineLength, file)) != -1) {
    if (strncmp(line, "v ", 2) == 0) {
      sscanf(line, "v %f %f %f\n", &v[vCount][0], &v[vCount][1], &v[vCount][2]);
      ++vCount;
    }
    else if (strncmp(line, "vt ", 3) == 0) {
      sscanf(line, "vt %f %f\n", &vt[vtCount][0], &vt[vtCount][1]);
      ++vtCount;
    }
    else if (strncmp(line, "vn ", 3) == 0) {
      sscanf(line, "vn %f %f %f\n", &vn[vnCount][0], &vn[vnCount][1], &vn[vnCount][2]);
      ++vnCount;
    }
    else if (strncmp(line, "f ", 2) == 0) {
      size_t idx[3][3];
      sscanf(line,
        "f %lu/%lu/%lu %lu/%lu/%lu %lu/%lu/%lu",
        &idx[0][0], &idx[0][1], &idx[0][2],
        &idx[1][0], &idx[1][1], &idx[1][2],
        &idx[2][0], &idx[2][1], &idx[2][2]
      );
      
      // copy data from temporary buffers to output buffer
      for (size_t i = 0; i < 3; ++i) {
        memcpy(buffer[fCount][i].v, v[idx[i][0]-1], sizeof(Vec3));
        memcpy(buffer[fCount][i].vt, vt[idx[i][1]-1], sizeof(Vec2));
        memcpy(buffer[fCount][i].vn, vn[idx[i][2]-1], sizeof(Vec3));
      }
      ++fCount;
    }
  }

  // free temporary buffers
  free(line);
  free(v);
  free(vt);
  free(vn);

  *vertexCount = 3 * fCount;
  return (float*)buffer;
};