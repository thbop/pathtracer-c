/*
* Copyright © 2025 Thbop
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the “Software”), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to do
* so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef MODEL_H
#define MODEL_H



// Reference:
// - https://en.wikipedia.org/wiki/Wavefront_.obj_file


#include "stdio.h"
#include "chelp/tstring.h"
#include "chelp/vector_t.h"
#include "chelp/list_t.h" // for qalloc
#include "vec3.h"

typedef struct {
    float x, y;
} vec2;

// A face/triangle used for building a Model
typedef struct {
    unsigned int
        vertex,
        tex_coord,
        normal;
} model_face_t;


typedef struct {
    char *name;
    vector_t vertices;
    vector_t tex_coords;
    vector_t faces;
    vector_t normals;
} model_t;


// Returns a new empty model build. `name` can be NULL
model_t *model_new( const char *name );

// Frees and deletes a model_t
void model_free( model_t *model );


// Loads the only object in a Wavefront .obj file (ensure that there is only
// one object). Also make sure that the faces are triangles.
model_t *model_load( const char *model_filename );


#endif