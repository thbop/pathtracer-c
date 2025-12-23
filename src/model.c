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


// Reference:
// - https://en.wikipedia.org/wiki/Wavefront_.obj_file


#include "model.h"


// Checks if an existing face matches the supplied face
// `searchDepth` determines how many elements starting from index 0 will be
// searched. This exists because this function is usually called before all
// Returns the index of the existing face if found, else returns -1
int _model_find_or_append_face( vector_t *face_list, model_face_t *face, int search_depth ) {
    int depth = face_list->elementCount;
    if ( search_depth > -1 && search_depth < depth )
        depth = search_depth;

    
    for ( int i = 0; i < depth; i++ ) {
        model_face_t *other = (model_face_t*)_vector_at( face_list, i );
        if (
            face->vertex == other->vertex     &&
            face->tex_coord == other->tex_coord
        )
            return i;
    }
    // Append if the face is not found
    model_face_t f = *face;
    _vector_append( face_list, &f );
    return -1;
}

// A CPU representation of the model to be loaded onto the GPU later
typedef struct {
    char *name;
    vector_t vertices;
    vector_t indices;
} Model;

// Returns a new empty model build. `name` can be NULL
model_t *model_new( const char *name, vec3 color ) {
    model_t model = {
        .name      = NULL,
        .color     = color,
        .vertices  = new_vector( vec3 ),
        .tex_coords = new_vector( vec2 ),
        .faces     = new_vector( model_face_t ),
    };
    if ( name != NULL )
        model.name = tstr_copy( (char*)name );
    
    return qalloc( model );
}


// Parses a single vertex line of a Wavefront .obj file and updates the model
// For example:
// v -1.000000 1.000000 0.000000
void _model_lex_vertex_line( char **args, model_t *model ) {
    switch ( args[0][1] ) {
        case '\0': { // v - vertex
            vec3 vertex = {
                strtof( args[1], NULL ),
                strtof( args[2], NULL ),
                strtof( args[3], NULL ),
            };
            vector_append( model->vertices, vertex );
            break;
        }
        case 'n': // vn - normal
            break;
        case 't': { // vt - texture coordinate
            vec2 tex_coord = {
                strtof( args[1], NULL ),
                strtof( args[2], NULL ),
            };
            vector_append( model->tex_coords, tex_coord );
            break;
        }
    }
}


// Parses a single face line of a Wavefront .obj file and updates the model
// Currently only uses the vertex index
// For example:
// f 2/1/1 3/2/1 1/3/1
void _model_lex_face_line( char **args, model_t *model ) {
    // Iterate through the three indices
    for ( int i = 1; i < 4; i++ ) {
        // Get vertex attributes:
        // vertex_index/texture_index/normal_index
        int attribute_count; // 3
        char **attributes = tstr_split( args[i], '/', &attribute_count );

        model_face_t face = {
            // .obj indcies start with 1 instead of 0
            .vertex    = atoi( attributes[0] ) - 1,
            .tex_coord = atoi( attributes[1] ) - 1,
            .normal    = atoi( attributes[2] ) - 1,
        };
        vector_append( model->faces, face );

        tstr_free_str_array( attributes, attribute_count );
    }
}

// Parses a single line of a Wavefront .obj file and updates the model
// accordingly. For example:
// ```
// o Plane
// v -1.000 -1.000 -0.000
// ```
void _model_lex_line( char *line, model_t *model ) {
    int argCount;
    char **args = tstr_split( line, ' ', &argCount );

    // If object name line, then copy only the first name
    if ( args[0][0] == 'o' && model->name == NULL )
        model->name = tstr_copy( args[1] );
    
    // If line is a vertex (v, vt, vn)
    else if ( args[0][0] == 'v' )
        _model_lex_vertex_line( args, model );
    
    // If line is a face (f)
    else if ( args[0][0] == 'f' )
        _model_lex_face_line( args, model );

    tstr_free_str_array( args, argCount );
}

// Frees and deletes a model_t
void model_free( model_t *model ) {
    if ( model == NULL ) return;

    if ( model->name != NULL )
        free( model->name );
    
    vector_free( model->vertices );
    vector_free( model->tex_coords );
    vector_free( model->faces );

    free( model );
}


// Loads the only object in a Wavefront .obj file (ensure that there is only
// one object). Also make sure that the faces are triangles.
model_t *model_load( const char *model_filename ) {
    FILE *fp = fopen( model_filename, "r" );
    if ( fp == NULL ) {
        fprintf( stderr, "Unable to open model file \"%s\"!\n", model_filename );
        return NULL;
    }
    fseek( fp, 0, SEEK_END );
    long data_size = ftell( fp );
    fseek( fp, 0, SEEK_SET );

    char *data = (char*)malloc( data_size );
    fread( data, sizeof(char), data_size, fp );
    fclose( fp );

    int line_count;
    char **lines = tstr_split( data, '\n', &line_count );

    model_t *model = model_new( NULL, (vec3){ 1.0f, 1.0f, 1.0f } );

    for ( int i = 0; i < line_count; i++ )
        _model_lex_line( lines[i], model );

    tstr_free_str_array( lines, line_count );
    free( data );

    return model;
}
