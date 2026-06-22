#include "obj_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool load_obj(const char *path, ModelData *out_model) {
    printf("Loading OBJ file %s...\n", path);

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file! Are you crazy brooooo ?\n");
        return false;
    }

  
    int temp_v_capacity = 1000, temp_vt_capacity = 1000, temp_vn_capacity = 1000;
    int v_count = 0, vt_count = 0, vn_count = 0;
    
    // We use a flat float array for simplicity (3 floats per vector, 2 per uv)
    float *temp_vertices = malloc(temp_v_capacity * 3 * sizeof(float));
    float *temp_uvs      = malloc(temp_vt_capacity * 2 * sizeof(float));
    float *temp_normals  = malloc(temp_vn_capacity * 3 * sizeof(float));


    int out_capacity = 1000;
    out_model->vertex_count = 0;
    out_model->vertices = malloc(out_capacity * 3 * sizeof(float));
    out_model->uvs      = malloc(out_capacity * 2 * sizeof(float));
    out_model->normals  = malloc(out_capacity * 3 * sizeof(float));

    char lineHeader[128];
    while (fscanf(file, "%s", lineHeader) != EOF) {

        if (strcmp(lineHeader, "v") == 0) {
            if (v_count >= temp_v_capacity) {
                temp_v_capacity *= 2;
                temp_vertices = realloc(temp_vertices, temp_v_capacity * 3 * sizeof(float));
            }
            fscanf(file, "%f %f %f\n", &temp_vertices[v_count*3], &temp_vertices[v_count*3+1], &temp_vertices[v_count*3+2]);
            v_count++;
        } 
        else if (strcmp(lineHeader, "vt") == 0) {
            if (vt_count >= temp_vt_capacity) {
                temp_vt_capacity *= 2;
                temp_uvs = realloc(temp_uvs, temp_vt_capacity * 2 * sizeof(float));
            }
            fscanf(file, "%f %f\n", &temp_uvs[vt_count*2], &temp_uvs[vt_count*2+1]);
            vt_count++;
        } 
        else if (strcmp(lineHeader, "vn") == 0) {
            if (vn_count >= temp_vn_capacity) {
                temp_vn_capacity *= 2;
                temp_normals = realloc(temp_normals, temp_vn_capacity * 3 * sizeof(float));
            }
            fscanf(file, "%f %f %f\n", &temp_normals[vn_count*3], &temp_normals[vn_count*3+1], &temp_normals[vn_count*3+2]);
            vn_count++;
        } 
        else if (strcmp(lineHeader, "f") == 0) {
            int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            
            if (matches != 9) {
                printf("File can't be read by our simple parser. Export as Triangles and include UVs and Normals!\n");
                fclose(file);
                return false;
            }

            // Expand final arrays if needed (3 vertices per face)
            if (out_model->vertex_count + 3 > out_capacity) {
                out_capacity *= 2;
                out_model->vertices = realloc(out_model->vertices, out_capacity * 3 * sizeof(float));
                out_model->uvs      = realloc(out_model->uvs, out_capacity * 2 * sizeof(float));
                out_model->normals  = realloc(out_model->normals, out_capacity * 3 * sizeof(float));
            }

           
            for (int i = 0; i < 3; i++) {
                int out_idx = out_model->vertex_count;
                
                // OBJ index starts at 1, C arrays start at 0
                int v_idx = vertexIndex[i] - 1;
                int vt_idx = uvIndex[i] - 1;
                int vn_idx = normalIndex[i] - 1;

                out_model->vertices[out_idx*3+0] = temp_vertices[v_idx*3+0];
                out_model->vertices[out_idx*3+1] = temp_vertices[v_idx*3+1];
                out_model->vertices[out_idx*3+2] = temp_vertices[v_idx*3+2];

                out_model->uvs[out_idx*2+0] = temp_uvs[vt_idx*2+0];
                out_model->uvs[out_idx*2+1] = temp_uvs[vt_idx*2+1]; 

                out_model->normals[out_idx*3+0] = temp_normals[vn_idx*3+0];
                out_model->normals[out_idx*3+1] = temp_normals[vn_idx*3+1];
                out_model->normals[out_idx*3+2] = temp_normals[vn_idx*3+2];

                out_model->vertex_count++;
            }
        } else {
            // Skip unrecognized lines
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    // Clean up temporary memory
    free(temp_vertices);
    free(temp_uvs);
    free(temp_normals);
    fclose(file);

    return true;
}

void free_model(ModelData *model) {
    if (model->vertices) free(model->vertices);
    if (model->uvs) free(model->uvs);
    if (model->normals) free(model->normals);
    model->vertex_count = 0;
}
