//
//  cppobjs.c
//  cppobjs
//
//  Created by Maxthon Chan on 13-4-10.
//  Copyright (c) 2013年 myWorld Creations. All rights reserved.
//

#include "vector.h"

// Private methods.

size_t vector_current_size;

void *vector_max_pointer(vector_t vec) 
{
    // Find the byte after the content ends
    assert(vec);
    void *base = vec -> data;
    size_t size = vec -> element_size * vec -> count;
    return base + size;
}

void vector_increase_pointer(vector_t vec, void **ptr) 
{
    // Increase the counter pointer
    assert(vec);
    size_t size = vec -> element_size;
    *ptr += size;
}

int vector_indexof_ptr(vector_t vec, const void *ptr)
{
    // Find the index of content pointer
    assert(vec);
    void *max = vector_max_pointer(vec);
    if (ptr >= max || ptr <= vec -> data)
        return -1;
    else
        return (int)((ptr - vec -> data) / vec -> element_size);
}

// Initializers

vector_t vector_init_empty(size_t size, size_t count)
{
    // Implemented using vector_init().
    return vector_init(NULL, size, count);
}

vector_t vector_init(const void *data, size_t size, size_t count)
{
    vector_t vec = NULL;
    void *alloc = NULL;
    
    assert(size);
    
    // Vectors are malloc'd.
    vec = malloc(sizeof(struct __vector_t));
    
    // Data are malloc'd too.
    alloc = calloc(size, count);
    
    // Any failed allocation will cause ditch.
    if (!alloc || !vec)
        return NULL;
    
    // Initialize all memory areas
    memset(vec, 0, sizeof(struct __vector_t));
    if (data)
        memcpy(alloc, data, size * count);
    else
        memset(alloc, 0, size * count);
    
    // Write output object.
    vec -> data = alloc;
    vec -> alloc_size = size * count;
    vec -> element_size = size;
    vec -> count = (data) ? count : 0;
    
    return vec;
}

// Methods.

ssize_t vector_indexof(vector_t vec, const void *object, equality_t eq)
{
    // Find the first occurance of object.
    void *buf = NULL;
    
    assert(vec);
    assert(object);
    assert(eq);
    
    buf = vector_alloc_object(vec);
    if (!buf)
        return VECTOR_NOT_FOUND;
    
    vector_current_size = vec -> element_size;
    
    for (size_t i = 0; i < vector_count(vec); i++)
    {
        vector_objectat(vec, buf, i);
        if (eq(buf, object))
            return i;
    }
    
    return VECTOR_NOT_FOUND;
}

size_t vector_count(vector_t vec)
{
    // Tell the vector size.
    assert(vec);
    return vec -> count;
}

void *vector_alloc_object(vector_t vec)
{
    // Allocate memory for a single object.
    void *block = NULL;
    
    assert(vec);
    
    block = malloc(vec -> element_size);
    if (block)
        memset(block, 0, vec -> element_size);
    
    return block;
}

void *vector_objectat(vector_t vec, void *buf, size_t index)
{
    // Give the object at the given size.
    
    assert(vec);
    assert(buf);
    assert(index < vec -> count); // Boundary check.
    
    void *ptr = vec -> data + index * vec -> element_size;
    
    return memcpy(buf, ptr, vec -> element_size);
}

bool vector_insert(vector_t vec, const void *object, size_t index)
{
    // Insert an object.
    void *ptr = NULL;
    void *ptrnext = NULL;
    
    assert(vec);
    assert(object);
    assert(index <= vec -> count);
    
    if (index == vec -> count)
        return vector_append(vec, object);
    
    if (vec -> count <= vec -> alloc_size / vec -> element_size)
        if (!vector_expand(vec, vec -> count + 1)) // Out of room. Expand it.
            return false;
    
    ptr = vec -> data + vec -> element_size * index;
    ptrnext = ptr + vec -> element_size;

    memmove(ptrnext, ptr, vector_max_pointer(vec) - ptr);
    memcpy(ptr, object, vec -> element_size);
    
    return true;
}

bool vector_append(vector_t vec, const void * object)
{
    // Append an object to the last.
    
    assert(vec);
    assert(object);
    
    if (vec -> count <= vec -> alloc_size / vec -> element_size)
        if (!vector_expand(vec, vec -> count + 1)) // Out of room. Expand it.
            return false;
    
    memcpy(vector_max_pointer(vec), object, vec -> element_size);
    vec -> count++;
    
    return true;
}

void vector_set(vector_t vec, const void *object, size_t index)
{
    // Assign an object
    
    void *ptr = NULL;
    
    assert(vec);
    assert(object);
    
    ptr = vec -> data + vec -> element_size * index;
    memcpy(ptr, object, vec -> element_size);
}

void vector_removeat(vector_t vec, size_t index)
{
    // Remove the object at the given index
    
    void *ptr = NULL;
    void *ptrnext = NULL;
    
    assert(vec);
    assert(index < vec -> count);
    
    ptr = vec -> data + (index * vec -> element_size);
    ptrnext = ptr + vec -> element_size;
    memmove(ptr, ptrnext, vector_max_pointer(vec) - ptrnext);
    vec -> count--;
}

void vector_removeall(vector_t vec, const void *object, equality_t eq)
{
    ssize_t idx = 0;
    void *buf = NULL;
    
    assert(vec);
    assert(object);
    assert(eq);
    
    buf = vector_alloc_object(vec);
    
    if (!buf)
        return; // We need this memory.
    
    vector_current_size = vec -> element_size;
    
    for (idx = vec -> count - 1; idx > 0; idx--)
    {
        if (vector_objectat(vec, buf, idx))
        {
            if (eq(buf, object))
                vector_removeat(vec, idx);
        }
    }
}

void vector_removefirst(vector_t vec, const void *object, equality_t eq)
{
    // Remove first occurance of object;
    
    ssize_t idx = 0;
    
    assert(vec);
    assert(object);
    assert(eq);
    
    vector_current_size = vec -> element_size;
    
    idx = vector_indexof(vec, object, eq);
    if (idx >= 0)
        vector_removeat(vec, idx);
}

void vector_sort(vector_t vec, comparator_t comp)
{
    qsort(vec -> data,
          vec -> count,
          vec -> element_size,
          comp);
}

bool vector_expand(vector_t vec, size_t alloc_size)
{
    // Expand or shrink the allocated memory to a given size.
    void *buf = NULL;
    size_t size;
    
    assert(vec);
    assert(alloc_size >= vec -> count);
    
    size = alloc_size * vec -> element_size;
    if (size < vec -> element_size)
        size = vec -> element_size;
    buf = malloc(size);
    
    if (!buf)
        return false;
    
    memset(buf, 0, size);
    memcpy(buf, vec -> data, vec -> alloc_size); // Move data over.
    
    vec -> data = buf;
    vec -> alloc_size = size;
    
    return true;
}

bool vector_compact(vector_t vec)
{
    // Reset the alloc size to its element count.
    assert(vec);
    
    return vector_expand(vec, vec -> count);
}

void vector_fini(vector_t vec)
{
    // We are done with it. Get rid of the whole thing.
    assert(vec);
    
    if (vec -> data)
        free(vec -> data);
    
    free(vec);
}

// Utility.

bool vector_binary_compare_internal(const void *r1, const void *r2)
{
    return !memcmp(r1, r2, vector_current_size);
}

equality_t vector_binary_compare(void)
{
    return vector_binary_compare_internal;
}
