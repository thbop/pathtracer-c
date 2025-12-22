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

#ifndef LIST_T_H
#define LIST_T_H


#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#ifndef QALLOC
#define QALLOC
void *_qalloc(void *value, size_t size);

// Given a stack-allocated variable (no literals), quick allocate will allocate
// and copy that variable to the heap, returning a pointer.
#define qalloc(value) \
    _qalloc(&value, sizeof(value))

#endif

// A list node
typedef struct {
    void *value;
    void *prev, *next;
} l_node;

// A list type
typedef struct {
    l_node *head;
    size_t len;
} list_t;

// Returns an empty new list
list_t new_list();


void *_list_append(list_t *list, void *value);

// Appends a stack-allocated variable to a given list.
#define list_append(list, value) \
    _list_append(&list, qalloc(value))

// Given a list and an iterator name, iterates through a list.
// You can access list value pointer by casting and dereferencing `it->value`
// Example:
//    `*(int*)int->value`
#define list_foreach(list, it) \
    for ( l_node *it = list.head; it != NULL; it = it->next )

// Same as `list_foreach`, but takes a list_t pointer.
#define list_ptr_foreach(list_ptr, it) \
    for ( l_node *it = list->head; it != NULL; it = it->next )

l_node *_list_find(list_t *list, void *value);

#define list_find(list, value_ptr) \
    _list_find(&list, value_ptr)

// Removes an element from the list, but does not free its value.
// Instead, it returns the value
void *_list_pop( list_t *list, l_node *popped );

void _list_remove(list_t *list, l_node *removed);

#define list_remove(list, item) \
    _list_remove(&list, item)

// Frees the list, but not its elements
void _list_disband( list_t *list );

void _list_free(list_t *list);

#define list_free( list ) \
    _list_free( &list )

#endif