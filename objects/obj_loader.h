#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <stdbool.h>

// 
typedef struct {
    float *vertices;    //  coordinates
    float *uvs;         // texture coordinates
    float *normals;     // ormal directions
    int vertex_count;   // total number of vertices to draw
} ModelData;


bool load_obj(const char *path, ModelData *out_model);


void free_model(ModelData *model);

#endif 
