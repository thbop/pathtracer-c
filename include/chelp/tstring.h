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

#ifndef TSTRING_H
#define TSTRING_H

// My custom string functions

#include "string.h"
#include "stdlib.h"

// Returns a heap-allocated substring given a string, a starting pos,
// and the number of characters to copy over (len).
// Returns NULL if the input is invalid
char *tstr_substr( char *string, int pos, int len );

// Returns a heap-allocated copy of the provided string
char *tstr_copy( char *string );

// Returns the number of times a particular character appears in a string.
int tstr_count_char( char *string, char character );

// Returns the index of the first occurrence of the character provided
// Returns -1 if the character is not found
int tstr_find( char *string, char character );

// Splits a given null-terminated string by a given char
// Returns an array of strings and the number of strings (stringCount)
// NOTE: Everything returned by this function is heap-allocated and must be
// freed by the user.
char **tstr_split( char *string, char separator, int *str_count );

// Frees a string array
// Ensure that stringCount reflects the actual size of the array or the program
// will seg fault.
void tstr_free_str_array( char **str_array, int str_count );

#endif