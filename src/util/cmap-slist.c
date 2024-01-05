
#include "cmap-slist.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

#define IMPL(NAME, name, type, dft) \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
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
/***************************************************************************** \
*****************************************************************************/ \
 \
static NAME##_CHUNK * name##_create_chunk(NAME##_INTERNAL * internal) \
{ \
  CMAP_MEM * mem = CMAP_KERNEL_MEM; \
  NAME##_CHUNK * chunk = (NAME##_CHUNK *)mem -> alloc( \
    sizeof(NAME##_CHUNK) + internal -> chunk_size * sizeof(type)); \
  chunk -> start = 0; \
  chunk -> stop = 0; \
  chunk -> size = 0; \
  chunk -> next = NULL; \
  chunk -> prev = NULL; \
  return chunk; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
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
  NAME##_CHUNK * chunk = internal -> first; \
  internal -> first = chunk -> next; \
  internal -> first -> prev = NULL; \
  if(chunk != internal -> no_free) CMAP_KERNEL_FREE(chunk); \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
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
  NAME##_CHUNK * chunk = internal -> last; \
  internal -> last = chunk -> prev; \
  internal -> last -> next = NULL; \
  if(chunk != internal -> no_free) CMAP_KERNEL_FREE(chunk); \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_push(CMAP_SLIST_##NAME * this, type v) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * last = internal -> last; \
  if(last -> size >= internal -> chunk_size) \
  { \
    name##_create_last(internal); \
    last = internal -> last; \
  } \
 \
  ((type *)(last + 1))[last -> stop] = v; \
 \
  cmap_util_public.inc_w_max(&last -> stop, internal -> chunk_size); \
  last -> size++; \
  internal -> size++; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static type name##_pop(CMAP_SLIST_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * last = internal -> last; \
  if(last -> size <= 0) return dft; \
  else \
  { \
    cmap_util_public.dec_w_max(&last -> stop, internal -> chunk_size); \
    type ret = ((type *)(last + 1))[last -> stop]; \
 \
    if((last -> size <= 1) && (last != internal -> first)) \
      name##_delete_last(internal); \
    else last -> size--; \
    internal -> size--; \
 \
    return ret; \
  } \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_unshift(CMAP_SLIST_##NAME * this, type v) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * first = internal -> first; \
  if(first -> size >= internal -> chunk_size) \
  { \
    name##_create_first(internal); \
    first = internal -> first; \
  } \
 \
  cmap_util_public.dec_w_max(&first -> start, internal -> chunk_size); \
  first -> size++; \
  internal -> size++; \
 \
  ((type *)(first + 1))[first -> start] = v; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static type name##_shift(CMAP_SLIST_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * first = internal -> first; \
  if(first -> size <= 0) return dft; \
  else \
  { \
    type ret = ((type *)(first + 1))[first -> start]; \
 \
    if((first -> size <= 1) && (first != internal -> last)) \
      name##_delete_first(internal); \
    else \
    { \
      cmap_util_public.inc_w_max(&first -> start, internal -> chunk_size); \
      first -> size--; \
    } \
    internal -> size--; \
 \
    return ret; \
  } \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static NAME##_CHUNK * name##_add_begin_get_chunk(NAME##_INTERNAL * internal, \
  int * i) \
{ \
  NAME##_CHUNK * chunk = internal -> first; \
  if(chunk -> size >= internal -> chunk_size) \
  { \
    name##_create_first(internal); \
    chunk = internal -> first; \
  } \
 \
  chunk -> size++; \
  internal -> size++; \
 \
  while(*i >= chunk -> size) \
  { \
    NAME##_CHUNK * next_chunk = chunk -> next; \
    ((type *)(chunk + 1))[chunk -> stop] = \
      ((type *)(next_chunk + 1))[next_chunk -> start]; \
    cmap_util_public.inc_w_max(&chunk -> stop, internal -> chunk_size); \
    cmap_util_public.inc_w_max(&next_chunk -> start, internal -> chunk_size); \
 \
    (*i) -= chunk -> size; \
    chunk = next_chunk; \
  } \
 \
  return chunk; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_add_begin(NAME##_INTERNAL * internal, int i, type v) \
{ \
  NAME##_CHUNK * chunk = name##_add_begin_get_chunk(internal, &i); \
 \
  cmap_util_public.dec_w_max(&chunk -> start, internal -> chunk_size); \
  int off = chunk -> start; \
  type * v_ptr = (type *)(chunk + 1); \
  for(; i > 0; i--) \
  { \
    int next_off = off; \
    cmap_util_public.inc_w_max(&next_off, internal -> chunk_size); \
    v_ptr[off] = v_ptr[next_off]; \
    off = next_off; \
  } \
 \
  v_ptr[off] = v; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static NAME##_CHUNK * name##_add_end_get_chunk(NAME##_INTERNAL * internal, \
  int * i) \
{ \
  NAME##_CHUNK * chunk = internal -> last; \
  if(chunk -> size >= internal -> chunk_size) \
  { \
    name##_create_last(internal); \
    chunk = internal -> last; \
  } \
 \
  chunk -> size++; \
  internal -> size++; \
 \
  while(*i >= chunk -> size) \
  { \
    cmap_util_public.dec_w_max(&chunk -> start, internal -> chunk_size); \
    NAME##_CHUNK * prev_chunk = chunk -> prev; \
    cmap_util_public.dec_w_max(&prev_chunk -> stop, internal -> chunk_size); \
    ((type *)(chunk + 1))[chunk -> start] = \
      ((type *)(prev_chunk + 1))[prev_chunk -> stop]; \
 \
    (*i) -= chunk -> size; \
    chunk = prev_chunk; \
  } \
 \
  return chunk; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_add_end(NAME##_INTERNAL * internal, int i, type v) \
{ \
  NAME##_CHUNK * chunk = name##_add_end_get_chunk(internal, &i); \
 \
  int off = chunk -> stop; \
  cmap_util_public.inc_w_max(&chunk -> stop, internal -> chunk_size); \
  type * v_ptr = (type *)(chunk + 1); \
  for(; i > 0; i--) \
  { \
    int next_off = off; \
    cmap_util_public.dec_w_max(&next_off, internal -> chunk_size); \
    v_ptr[off] = v_ptr[next_off]; \
    off = next_off; \
  } \
 \
  v_ptr[off] = v; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_add(CMAP_SLIST_##NAME * this, int i, type v) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  if((i >= 0) && (i <= internal -> size)) \
  { \
    if(i <= (internal -> size >> 1)) name##_add_begin(internal, i, v); \
    else name##_add_end(internal, internal -> size - i, v); \
  } \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_rm_begin_upd_chunk(NAME##_INTERNAL * internal, \
  NAME##_CHUNK * chunk) \
{ \
  NAME##_CHUNK * first = internal -> first; \
  while(chunk != first) \
  { \
    cmap_util_public.dec_w_max(&chunk -> start, internal -> chunk_size); \
    NAME##_CHUNK * prev_chunk = chunk -> prev; \
    cmap_util_public.dec_w_max(&prev_chunk -> stop, internal -> chunk_size); \
    ((type *)(chunk + 1))[chunk -> start] = \
      ((type *)(prev_chunk + 1))[prev_chunk -> stop]; \
    chunk = prev_chunk; \
  } \
 \
  if((first -> size <= 1) && (first != internal -> last)) \
    name##_delete_first(internal); \
  else first -> size--; \
  internal -> size--; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static NAME##_CHUNK * name##_rm_begin_get_chunk(NAME##_INTERNAL * internal, \
  int * i) \
{ \
  NAME##_CHUNK * chunk = internal -> first; \
  while(*i >= chunk -> size) \
  { \
    (*i) -= chunk -> size; \
    chunk = chunk -> next; \
  } \
  return chunk; \
} \
 \
static type name##_rm_begin(NAME##_INTERNAL * internal, int i) \
{ \
  NAME##_CHUNK * chunk = name##_rm_begin_get_chunk(internal, &i); \
 \
  int off = chunk -> start; \
  cmap_util_public.inc_w_max(&chunk -> start, internal -> chunk_size); \
  cmap_util_public.add_w_max(&off, i, internal -> chunk_size); \
  type * v_ptr = (type *)(chunk + 1); \
  type v = v_ptr[off]; \
  for(; i > 0; i--) \
  { \
    int next_off = off; \
    cmap_util_public.dec_w_max(&next_off, internal -> chunk_size); \
    v_ptr[off] = v_ptr[next_off]; \
    off = next_off; \
  } \
 \
  name##_rm_begin_upd_chunk(internal, chunk); \
 \
  return v; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_rm_end_upd_chunk(NAME##_INTERNAL * internal, \
  NAME##_CHUNK * chunk) \
{ \
  NAME##_CHUNK * last = internal -> last; \
  while(chunk != last) \
  { \
    NAME##_CHUNK * next_chunk = chunk -> next; \
    ((type *)(chunk + 1))[chunk -> stop] = \
      ((type *)(next_chunk + 1))[next_chunk -> start]; \
    cmap_util_public.inc_w_max(&next_chunk -> start, internal -> chunk_size); \
    cmap_util_public.inc_w_max(&chunk -> stop, internal -> chunk_size); \
    chunk = next_chunk; \
  } \
 \
  if((last -> size <= 1) && (last != internal -> first)) \
    name##_delete_last(internal); \
  else last -> size--; \
  internal -> size--; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static NAME##_CHUNK * name##_rm_end_get_chunk(NAME##_INTERNAL * internal, \
  int * i) \
{ \
  NAME##_CHUNK * chunk = internal -> last; \
  while(*i >= chunk -> size) \
  { \
    (*i) -= chunk -> size; \
    chunk = chunk -> prev; \
  } \
  return chunk; \
} \
 \
static type name##_rm_end(NAME##_INTERNAL * internal, int i) \
{ \
  NAME##_CHUNK * chunk = name##_rm_end_get_chunk(internal, &i); \
 \
  cmap_util_public.dec_w_max(&chunk -> stop, internal -> chunk_size); \
  int off = chunk -> stop; \
  cmap_util_public.rm_w_max(&off, i, internal -> chunk_size); \
  type * v_ptr = (type *)(chunk + 1); \
  type v = v_ptr[off]; \
  for(; i > 0; i--) \
  { \
    int next_off = off; \
    cmap_util_public.inc_w_max(&next_off, internal -> chunk_size); \
    v_ptr[off] = v_ptr[next_off]; \
    off = next_off; \
  } \
 \
  name##_rm_end_upd_chunk(internal, chunk); \
 \
  return v; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static type name##_rm(CMAP_SLIST_##NAME * this, int i) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  if((i < 0) || (i >= internal -> size)) return dft; \
  else \
  { \
    if(i < (internal -> size >> 1)) return name##_rm_begin(internal, i); \
    else return name##_rm_end(internal, internal -> size - 1 - i); \
  } \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static NAME##_CHUNK * name##_get_chunk(NAME##_INTERNAL * internal, int * i) \
{ \
  NAME##_CHUNK * chunk = internal -> first; \
 \
  if(chunk -> size == 0) return NULL; \
 \
  while((chunk != NULL) && (*i >= chunk -> size)) \
  { \
    (*i) -= chunk -> size; \
    chunk = chunk -> next; \
  } \
  return chunk; \
} \
 \
static type * name##_get(CMAP_SLIST_##NAME * this, int i) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * chunk = name##_get_chunk(internal, &i); \
  if(chunk == NULL) return NULL; \
  else \
  { \
    int off = chunk -> start; \
    cmap_util_public.add_w_max(&off, i, internal -> chunk_size); \
    return ((type *)(chunk + 1)) + off; \
  } \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static type * name##_first(CMAP_SLIST_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * first = internal -> first; \
  if(first -> size <= 0) return NULL; \
  else return ((type *)(first + 1)) + first -> start; \
} \
 \
static type * name##_last(CMAP_SLIST_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * last = internal -> last; \
  if(last -> size <= 0) return NULL; \
  else \
  { \
    int stop = last -> stop; \
    cmap_util_public.dec_w_max(&stop, internal -> chunk_size); \
    return ((type *)(last + 1)) + stop; \
  } \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static int name##_size(CMAP_SLIST_##NAME * this) \
{ \
  return ((NAME##_INTERNAL *)(this + 1)) -> size; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_clean(CMAP_SLIST_##NAME * this) \
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
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_apply(CMAP_SLIST_##NAME * this, \
  CMAP_SLIST_##NAME##_APPLY_FN fn, void * data) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * chunk = internal -> first; \
  while(chunk != NULL) \
  { \
    if(chunk -> size > 0) \
    { \
      int off = chunk -> start; \
      type * v_ptr = (type *)(chunk + 1); \
      /* It is possible that start == stop */ \
      fn(v_ptr + off, data); \
      cmap_util_public.inc_w_max(&off, internal -> chunk_size); \
      while(off != chunk -> stop) \
      { \
        fn(v_ptr + off, data); \
        cmap_util_public.inc_w_max(&off, internal -> chunk_size); \
      } \
    } \
    chunk = chunk -> next; \
  } \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_delete_all_chunk(CMAP_SLIST_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * chunk = internal -> first; \
  while(chunk != NULL) \
  { \
    NAME##_CHUNK * tmp = chunk; \
    chunk = chunk -> next; \
    if(tmp != internal -> no_free) CMAP_KERNEL_FREE(tmp); \
  } \
} \
 \
static void name##_delete(CMAP_SLIST_##NAME * this) \
{ \
  name##_delete_all_chunk(this); \
  CMAP_KERNEL_FREE(this); \
} \
 \
static void name##_init(CMAP_SLIST_##NAME * this, int chunk_size) \
{ \
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
  this -> add = name##_add; \
  this -> rm = name##_rm; \
  this -> get = name##_get; \
  this -> first = name##_first; \
  this -> last = name##_last; \
  this -> size = name##_size; \
  this -> clean = name##_clean; \
  this -> apply = name##_apply; \
} \
 \
static int name##_sizeof(int chunk_size) \
{ \
  return sizeof(CMAP_SLIST_##NAME) + sizeof(NAME##_INTERNAL) + \
    sizeof(NAME##_CHUNK) + chunk_size * sizeof(type); \
} \
 \
static CMAP_SLIST_##NAME * name##_create(int chunk_size) \
{ \
  chunk_size = (chunk_size == 0) ? 1 << 10 : chunk_size; \
 \
  CMAP_MEM * mem = CMAP_KERNEL_MEM; \
  CMAP_SLIST_##NAME * this = (CMAP_SLIST_##NAME *) \
    mem -> alloc(name##_sizeof(chunk_size)); \
  name##_init(this, chunk_size); \
  return this; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
const CMAP_SLIST_##NAME##_PUBLIC cmap_slist_##name##_public = { name##_create };

/*******************************************************************************
*******************************************************************************/

CMAP_SLIST_LOOP(IMPL)
