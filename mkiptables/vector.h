/*
 *  cppobjs.h
 *  cppobjs
 *
 *  Created by Maxthon Chan on 13-4-10.
 *  Copyright (c) 2013年 myWorld Creations. All rights reserved.
 *
 */

#ifndef cppobjs_
#define cppobjs_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#if defined(__cplusplus)
#define VECTOR_BEGIN_DECLS extern "C" {
#define VECTOR_END_DECLS }
#define VECTOR_EXTERN extern "C"
#else
#define VECTOR_BEGIN_DECLS
#define VECTOR_END_DECLS
#define VECTOR_EXTERN extern
#endif

VECTOR_BEGIN_DECLS

typedef struct __vector_t
{
    void *data;
    size_t alloc_size;
    size_t element_size;
    size_t count;
} *vector_t;

typedef enum __comparison_result
{
    compare_ascending = -1,
    compare_same = 0,
    compare_descending = 1
} comparison_result;

typedef bool                (*equality_t)   (const void *, const void *);
typedef comparison_result   (*comparator_t) (const void *, const void *);

#define VECTOR_NOT_FOUND (-1)

VECTOR_EXTERN equality_t vector_binary_compare(void);

//            type        method name         object      data to mind    other

// Allocators
VECTOR_EXTERN vector_t    vector_init_empty                              (size_t, size_t);
VECTOR_EXTERN vector_t    vector_init                    (const void *,   size_t, size_t);

// Methods
VECTOR_EXTERN ssize_t     vector_indexof      (vector_t,  const void *,   equality_t);
VECTOR_EXTERN size_t      vector_count        (vector_t);
VECTOR_EXTERN void *      vector_alloc_object (vector_t);
VECTOR_EXTERN void *      vector_objectat     (vector_t,        void *,   size_t);
VECTOR_EXTERN bool        vector_insert       (vector_t,  const void *,   size_t);
VECTOR_EXTERN bool        vector_append       (vector_t,  const void *);
VECTOR_EXTERN void        vector_set          (vector_t,  const void *,   size_t);
VECTOR_EXTERN void        vector_removeat     (vector_t,                  size_t);
VECTOR_EXTERN void        vector_removeall    (vector_t,  const void *,   equality_t);
VECTOR_EXTERN void        vector_removefirst  (vector_t,  const void *,   equality_t);
VECTOR_EXTERN void        vector_sort         (vector_t,                  comparator_t);
VECTOR_EXTERN bool        vector_expand       (vector_t,                  size_t);
VECTOR_EXTERN bool        vector_compact      (vector_t);

// Deallocator
VECTOR_EXTERN void        vector_fini         (vector_t);

#endif
