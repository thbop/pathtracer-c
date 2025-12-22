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

#ifndef VECTOR_T_H
#define VECTOR_T_H

#include "stdlib.h"
#include "string.h"


#define VECTOR_T_START_BUFFER_SIZE 16

// Dynamically resizable buffer
typedef struct {
    unsigned int
        elementSize,  // sizeof(ElementType)
        elementCount, // <= bufferSize
        bufferSize;   // Always a multiple of 2
    
    unsigned char *buffer;
} vector_t;

// Creates a new vector_t given the sizeof() the element type to be stored
vector_t _new_vector( int elementSize );

// Creates a new vector_t given the element type
#define new_vector( type ) \
    _new_vector( sizeof(type) )


// Appends value to a vector and returns its index
// Returns -1 if something went wrong
int _vector_append( vector_t *vector, void *value );

// Appends a value to a vector
// Ensure you are appending a value of this same type as the vector. Does not
// accept literals
#define vector_append( vector, value ) \
    _vector_append( &(vector), &(value) )

// Inserts a value at a particular index. This does not replace the value
// that currently resides at that index, instead makes room for the value by
// shifting other values upward
void _vector_insert( vector_t *vector, unsigned int index, void *value );

// Inserts a value at a particular index. This does not replace the value
// that currently resides at that index, instead makes room for the value by
// shifting other values upward
#define vector_insert( vector, index, value ) \
    _vector_insert( &(vector), index, &(value) )

// Returns a pointer to a particular index
// Use this instead of directly indexing the vector->buffer
void *_vector_at( vector_t *vector, unsigned int index );

// Macro-pointer tricks to access a particular element by index given the
// vector type, the vector, and the index
#define vector_at( type, vector, index ) \
    ( *(type*)_vector_at( &(vector), index ) )

// "Returns" the size in bytes of all the elements
#define vector_sizeof_elements( vector ) \
    ( (vector).elementSize * (vector).elementCount )

// Searches the vector for the provided value
// Returns the index of the value. -1 if the value is not found.
int _vector_find( vector_t *vector, void *value );

// Searches for the provided value. Returns the index or -1 if it is not found
// Do not search for literals and ensure you are searching for a value of the
// same type as the vector
#define vector_find( vector, value ) \
    _vector_find( &(vector), &(value) )

void _vector_remove_index( vector_t *vector, unsigned int index );

// Removes the value at the given index from the vector
#define vector_remove_index( vector, index ) \
    _vector_remove_index( &(vector), index )

// Finds a value and removes it if possible
void _vector_remove( vector_t *vector, void *value );

// Removes the provided value from the vector
// Do not provide literals and only remove values of the same type as the
// vector
#define vector_remove( vector, value ) \
    _vector_remove( &(vector), &(value) )

// Frees a vector's heap-allocated resources
void _vector_free( vector_t *vector );

// Frees a vector's heap-allocated resources
#define vector_free( vector ) \
    _vector_free( &(vector) )

#endif