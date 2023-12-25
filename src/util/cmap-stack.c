
#include "cmap-stack.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

#define IMPL(NAME, name, TYPE) \
typedef struct NAME##_CHUNK NAME##_CHUNK; \
 \
struct NAME##_CHUNK \
{ \
  int start, stop, size; \
  NAME##_CHUNK * next, * prev; \
}; \
 \
typedef struct \
{ \
  NAME##_CHUNK * first, * last, * no_free; \
  int size, chunk_size; \
} NAME##_INTERNAL; \
 \
static NAME##_CHUNK * name##_create_chunk(NAME##_INTERNAL * internal) \
{ \
  CMAP_MEM * mem = CMAP_KERNEL_MEM; \
  NAME##_CHUNK * chunk = (NAME##_CHUNK *)mem -> alloc( \
    sizeof(NAME##_CHUNK) + internal -> chunk_size * sizeof(TYPE)); \
  chunk -> start = 0; \
  chunk -> stop = 0; \
  chunk -> size = 0; \
  chunk -> next = NULL; \
  chunk -> prev = NULL; \
  return chunk; \
} \
 \
static void name##_create_first(NAME##_INTERNAL * internal) \
{ \
  NAME##_CHUNK * chunk = name##_create_chunk(internal); \
  chunk -> next = internal -> first; \
  internal -> first -> prev = chunk; \
  internal -> first = chunk; \
} \
 \
static void name##_delete_first(NAME##_INTERNAL * internal) \
{ \
  NAME##_CHUNK * chunk = internal -> first, * first = chunk -> next; \
  first -> prev = NULL; \
  internal -> first = first; \
  if(chunk != internal -> no_free) CMAP_KERNEL_FREE(chunk); \
} \
 \
static void name##_create_last(NAME##_INTERNAL * internal) \
{ \
  NAME##_CHUNK * chunk = name##_create_chunk(internal); \
  chunk -> prev = internal -> last; \
  internal -> last -> next = chunk; \
  internal -> last = chunk; \
} \
 \
static void name##_delete_last(NAME##_INTERNAL * internal) \
{ \
  NAME##_CHUNK * chunk = internal -> last, * last = chunk -> prev; \
  last -> next = NULL; \
  internal -> last = last; \
  if(chunk != internal -> no_free) CMAP_KERNEL_FREE(chunk); \
} \
 \
static TYPE * name##_push(CMAP_STACK_##NAME * this, TYPE v) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  if(internal -> last -> size >= internal -> chunk_size) \
    name##_create_last(internal); \
  NAME##_CHUNK * last = internal -> last; \
  TYPE * ret = ((TYPE *)(last + 1)) + last -> stop; \
  *ret = v; \
  cmap_util_public.inc_w_max(&last -> stop, internal -> chunk_size); \
  last -> size++; \
  internal -> size++; \
  return ret; \
} \
 \
static TYPE * name##_pop(CMAP_STACK_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * last = internal -> last; \
  if(last -> size <= 0) return NULL; \
  else \
  { \
    cmap_util_public.dec_w_max(&last -> stop, internal -> chunk_size); \
    TYPE * ret = ((TYPE *)(last + 1)) + last -> stop; \
    if((last -> size <= 1) && (last != internal -> first)) \
      name##_delete_last(internal); \
    else last -> size--; \
    internal -> size--; \
    return ret; \
  } \
} \
 \
static TYPE * name##_unshift(CMAP_STACK_##NAME * this, TYPE v) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  if(internal -> first -> size >= internal -> chunk_size) \
    name##_create_first(internal); \
  NAME##_CHUNK * first = internal -> first; \
  cmap_util_public.dec_w_max(&first -> start, internal -> chunk_size); \
  first -> size++; \
  internal -> size++; \
  TYPE * ret = ((TYPE *)(first + 1)) + first -> start; \
  *ret = v; \
  return ret; \
} \
 \
static TYPE * name##_shift(CMAP_STACK_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * first = internal -> first; \
  if(first -> size <= 0) return NULL; \
  else \
  { \
    TYPE * ret = ((TYPE *)(first + 1)) + first -> start; \
    if((first -> size <= 1) && (first != internal -> last)) \
      name##_delete_first(internal); \
    else \
    { \
      cmap_util_public.inc_w_max(&first -> start, internal -> chunk_size); \
      first -> size--; \
    } \
    internal -> size--; \
    return ret; \
  } \
} \
 \
static TYPE * name##_first(CMAP_STACK_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * first = internal -> first; \
  if(first -> size <= 0) return NULL; \
  else return ((TYPE *)(first + 1)) + first -> start; \
} \
 \
static TYPE * name##_last(CMAP_STACK_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * last = internal -> last; \
  if(last -> size <= 0) return NULL; \
  else \
  { \
    int stop = last -> stop; \
    cmap_util_public.dec_w_max(&stop, internal -> chunk_size); \
    return ((TYPE *)(last + 1)) + stop; \
  } \
} \
 \
static int name##_size(CMAP_STACK_##NAME * this) \
{ \
  return ((NAME##_INTERNAL *)(this + 1)) -> size; \
} \
 \
static void name##_clean(CMAP_STACK_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  while(internal -> first != internal -> last) name##_delete_last(internal); \
  NAME##_CHUNK * first = internal -> first; \
  first -> start = 0; \
  first -> stop = 0; \
  first -> size = 0; \
  internal -> size = 0; \
} \
 \
static void name##_apply(CMAP_STACK_##NAME * this, \
  CMAP_STACK_##NAME##_APPLY_FN fn, void * data) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * cur = internal -> first; \
  while(cur != NULL) \
  { \
    if(cur -> size > 0) \
    { \
      int off = cur -> start; \
      fn(((TYPE *)(cur + 1)) + off, data); \
      cmap_util_public.inc_w_max(&off, internal -> chunk_size); \
      while(off != cur -> stop) \
      { \
        fn(((TYPE *)(cur + 1)) + off, data); \
        cmap_util_public.inc_w_max(&off, internal -> chunk_size); \
      } \
    } \
    cur = cur -> next; \
  } \
} \
 \
static void name##_delete(CMAP_STACK_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * chunk = internal -> first; \
  while(chunk != NULL) \
  { \
    NAME##_CHUNK * tmp = chunk; \
    chunk = chunk -> next; \
    if(tmp != internal -> no_free) CMAP_KERNEL_FREE(tmp); \
  } \
  CMAP_KERNEL_FREE(this); \
} \
 \
static CMAP_STACK_##NAME * name##_create(int chunk_size) \
{ \
  chunk_size = (chunk_size == 0) ? 1 << 10 : chunk_size; \
 \
  CMAP_MEM * mem = CMAP_KERNEL_MEM; \
  CMAP_STACK_##NAME * this = (CMAP_STACK_##NAME *)mem -> alloc( \
    sizeof(CMAP_STACK_##NAME) + sizeof(NAME##_INTERNAL) + \
    sizeof(NAME##_CHUNK) + chunk_size * sizeof(TYPE)); \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * first = (NAME##_CHUNK *)(internal + 1); \
 \
  first -> start = 0; \
  first -> stop = 0; \
  first -> size = 0; \
  first -> next = NULL; \
  first -> prev = NULL; \
 \
  internal -> first = first; \
  internal -> last = first; \
  internal -> no_free = first; \
  internal -> size = 0; \
  internal -> chunk_size = chunk_size; \
 \
  this -> delete = name##_delete; \
  this -> push = name##_push; \
  this -> pop = name##_pop; \
  this -> unshift = name##_unshift; \
  this -> shift = name##_shift; \
  this -> first = name##_first; \
  this -> last = name##_last; \
  this -> size = name##_size; \
  this -> clean = name##_clean; \
  this -> apply = name##_apply; \
 \
  return this; \
} \
 \
const CMAP_STACK_##NAME##_PUBLIC cmap_stack_##name##_public = \
{ \
  name##_create \
};

/*******************************************************************************
*******************************************************************************/

IMPL(LC, lc, CMAP_LIFECYCLE *)
IMPL(LC_PTR, lc_ptr, CMAP_LIFECYCLE **)
IMPL(PROC_CTX, proc_ctx, CMAP_PROC_CTX *)
IMPL(CHAR_PTR, char_ptr, char *)
