#ifndef __CMAP_SLIST_DEFINE_H__
#define __CMAP_SLIST_DEFINE_H__

#include <stdlib.h>
#include "cmap-lifecycle-type.h"
#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-mem.h"
#include "cmap-config.h"
#include "cmap.h"

/*******************************************************************************
*******************************************************************************/

#define CMAP_SLIST_LOOP(macro) \
  macro(LC, lc, CMAP_LIFECYCLE *, NULL) \
  macro(LC_PTR, lc_ptr, CMAP_LIFECYCLE **, NULL) \
  macro(MAP, map, CMAP_MAP *, NULL) \
  macro(CHAR_PTR, char_ptr, char *, NULL)

/*******************************************************************************
*******************************************************************************/

#define CMAP_SLIST_STRUCT_DECL(NAME, type) \
typedef struct CMAP_SLIST_##NAME CMAP_SLIST_##NAME; \
 \
typedef void (*CMAP_SLIST_##NAME##_APPLY_FN)(type * v, void * data); \
 \
struct CMAP_SLIST_##NAME \
{ \
  void (*delete)(CMAP_SLIST_##NAME * this); \
 \
  void (*push)(CMAP_SLIST_##NAME * this, type v); \
  type (*pop)(CMAP_SLIST_##NAME * this); \
 \
  void (*unshift)(CMAP_SLIST_##NAME * this, type v); \
  type (*shift)(CMAP_SLIST_##NAME * this); \
 \
  char (*add)(CMAP_SLIST_##NAME * this, int i, type v); \
  type (*rm)(CMAP_SLIST_##NAME * this, int i); \
 \
  type * (*get)(CMAP_SLIST_##NAME * this, int i); \
  type * (*first)(CMAP_SLIST_##NAME * this); \
  type * (*last)(CMAP_SLIST_##NAME * this); \
 \
  int (*size)(CMAP_SLIST_##NAME * this); \
 \
  void (*apply)(CMAP_SLIST_##NAME * this, CMAP_SLIST_##NAME##_APPLY_FN fn, \
    void * data); \
 \
  void (*clean)(CMAP_SLIST_##NAME * this); \
};

/*******************************************************************************
*******************************************************************************/

#define CMAP_SLIST_DECL(NAME, name, type, dft) \
CMAP_SLIST_STRUCT_DECL(NAME, type) \
 \
typedef struct \
{ \
  CMAP_SLIST_##NAME * (*create)(int chunk_size); \
} CMAP_SLIST_##NAME##_PUBLIC; \
 \
extern const CMAP_SLIST_##NAME##_PUBLIC cmap_slist_##name##_public;

/*******************************************************************************
*******************************************************************************/

#define CMAP_SLIST_STATIC_FN_IMPL(NAME, name, type, dft) \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
typedef struct NAME##_CHUNK NAME##_CHUNK; \
 \
struct NAME##_CHUNK \
{ \
  type * first; \
  type * last; \
  type * start; \
  type * stop; \
  int size; \
  NAME##_CHUNK * next, * prev; \
}; \
 \
typedef struct \
{ \
  NAME##_CHUNK * first, * last; \
  int size, chunk_size; \
  char used; \
} NAME##_INTERNAL; \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static inline void name##_inc_w_last(type ** p, NAME##_CHUNK * chunk) \
{ \
  (*p)++; \
  if(*p > chunk -> last) *p = chunk -> first; \
} \
 \
static inline void name##_dec_w_last(type ** p, NAME##_CHUNK * chunk) \
{ \
  (*p)--; \
  if(*p < chunk -> first) *p = chunk -> last; \
} \
 \
static inline void name##_add_w_last(type ** p, int i, NAME##_CHUNK * chunk) \
{ \
  (*p) += i; \
  if(*p > chunk -> last) *p = chunk -> first + ((*p - chunk -> last) - 1); \
} \
 \
static inline void name##_rm_w_last(type ** p, int i, NAME##_CHUNK * chunk) \
{ \
  (*p) -= i; \
  if(*p < chunk -> first) *p = chunk -> last - ((chunk -> first - *p) - 1); \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static NAME##_CHUNK * name##_create_chunk(NAME##_INTERNAL * internal) \
{ \
  NAME##_CHUNK * chunk; \
  type * first; \
  if(internal -> used) \
  { \
    int chunk_size = internal -> chunk_size; \
    chunk = (NAME##_CHUNK *)CMAP_MEM_INSTANCE -> alloc( \
      sizeof(NAME##_CHUNK) + chunk_size * sizeof(type)); \
    first = (type *)(chunk + 1); \
    chunk -> first = first; \
    chunk -> last = first + (chunk_size - 1); \
  } \
  else \
  { \
    internal -> used = CMAP_T; \
    chunk = (NAME##_CHUNK *)(internal + 1); \
    first = (type *)(chunk + 1); \
  } \
  chunk -> start = first; \
  chunk -> stop = first; \
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
  if(chunk == (NAME##_CHUNK *)(internal + 1)) internal -> used = CMAP_F; \
  else CMAP_MEM_INSTANCE_FREE(chunk); \
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
  if(chunk == (NAME##_CHUNK *)(internal + 1)) internal -> used = CMAP_F; \
  else CMAP_MEM_INSTANCE_FREE(chunk); \
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
  *last -> stop = v; \
 \
  name##_inc_w_last(&last -> stop, last); \
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
    name##_dec_w_last(&last -> stop, last); \
    type ret = *last -> stop; \
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
  name##_dec_w_last(&first -> start, first); \
  first -> size++; \
  internal -> size++; \
 \
  *first -> start = v; \
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
    type ret = *first -> start; \
 \
    if((first -> size <= 1) && (first != internal -> last)) \
      name##_delete_first(internal); \
    else \
    { \
      name##_inc_w_last(&first -> start, first); \
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
    *chunk -> stop = *next_chunk -> start; \
    name##_inc_w_last(&chunk -> stop, chunk); \
    name##_inc_w_last(&next_chunk -> start, next_chunk); \
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
  name##_dec_w_last(&chunk -> start, chunk); \
  type * cur = chunk -> start; \
  for(; i > 0; i--) \
  { \
    type * next = cur; \
    name##_inc_w_last(&next, chunk); \
    *cur = *next; \
    cur = next; \
  } \
 \
  *cur = v; \
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
    name##_dec_w_last(&chunk -> start, chunk); \
    NAME##_CHUNK * prev_chunk = chunk -> prev; \
    name##_dec_w_last(&prev_chunk -> stop, prev_chunk); \
    *chunk -> start = *prev_chunk -> stop; \
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
  type * cur = chunk -> stop; \
  name##_inc_w_last(&chunk -> stop, chunk); \
  for(; i > 0; i--) \
  { \
    type * next = cur; \
    name##_dec_w_last(&next, chunk); \
    *cur = *next; \
    cur = next; \
  } \
 \
  *cur = v; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static char name##_add(CMAP_SLIST_##NAME * this, int i, type v) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  if((i >= 0) && (i <= internal -> size)) \
  { \
    if(i <= (internal -> size >> 1)) name##_add_begin(internal, i, v); \
    else name##_add_end(internal, internal -> size - i, v); \
    return CMAP_T; \
  } \
  return CMAP_F; \
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
    name##_dec_w_last(&chunk -> start, chunk); \
    NAME##_CHUNK * prev_chunk = chunk -> prev; \
    name##_dec_w_last(&prev_chunk -> stop, prev_chunk); \
    *chunk -> start = *prev_chunk -> stop; \
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
  type * cur = chunk -> start; \
  name##_inc_w_last(&chunk -> start, chunk); \
  name##_add_w_last(&cur, i, chunk); \
  type v = *cur; \
  for(; i > 0; i--) \
  { \
    type * next = cur; \
    name##_dec_w_last(&next, chunk); \
    *cur = *next; \
    cur = next; \
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
    *chunk -> stop = *next_chunk -> start; \
    name##_inc_w_last(&next_chunk -> start, next_chunk); \
    name##_inc_w_last(&chunk -> stop, chunk); \
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
  name##_dec_w_last(&chunk -> stop, chunk); \
  type * cur = chunk -> stop; \
  name##_rm_w_last(&cur, i, chunk); \
  type v = *cur; \
  for(; i > 0; i--) \
  { \
    type * next = cur; \
    name##_inc_w_last(&next, chunk); \
    *cur = *next; \
    cur = next; \
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
    type * cur = chunk -> start; \
    name##_add_w_last(&cur, i, chunk); \
    return cur; \
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
  else return first -> start; \
} \
 \
static type * name##_last(CMAP_SLIST_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * last = internal -> last; \
  if(last -> size <= 0) return NULL; \
  else \
  { \
    type * cur = last -> stop; \
    name##_dec_w_last(&cur, last); \
    return cur; \
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
static void name##_apply(CMAP_SLIST_##NAME * this, \
  CMAP_SLIST_##NAME##_APPLY_FN fn, void * data) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * chunk = internal -> first; \
  while(chunk != NULL) \
  { \
    if(chunk -> size > 0) \
    { \
      type * cur = chunk -> start; \
      /* It is possible that start == stop */ \
      fn(cur, data); \
      name##_inc_w_last(&cur, chunk); \
      while(cur != chunk -> stop) \
      { \
        fn(cur, data); \
        name##_inc_w_last(&cur, chunk); \
      } \
    } \
    chunk = chunk -> next; \
  } \
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
  type * first_first = first -> first; \
  first -> start = first_first; \
  first -> stop = first_first; \
  first -> size = 0; \
  internal -> size = 0; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_delete_chunks(CMAP_SLIST_##NAME * this) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * no_free = (NAME##_CHUNK *)(internal + 1), \
    * chunk = internal -> first; \
  while(chunk != NULL) \
  { \
    NAME##_CHUNK * tmp = chunk; \
    chunk = chunk -> next; \
    if(tmp != no_free) CMAP_MEM_INSTANCE_FREE(tmp); \
  } \
} \
 \
static void name##_delete(CMAP_SLIST_##NAME * this) \
{ \
  name##_delete_chunks(this); \
  CMAP_MEM_INSTANCE_FREE(this); \
} \
 \
static void name##_init(CMAP_SLIST_##NAME * this, int chunk_size) \
{ \
  NAME##_INTERNAL * internal = (NAME##_INTERNAL *)(this + 1); \
  NAME##_CHUNK * first = (NAME##_CHUNK *)(internal + 1); \
  type * first_first = (type *)(first + 1); \
 \
  first -> first = first_first; \
  first -> last = first_first + (chunk_size - 1); \
  first -> start = first_first; \
  first -> stop = first_first; \
  first -> size = 0; \
  first -> next = NULL; \
  first -> prev = NULL; \
 \
  internal -> first = first; \
  internal -> last = first; \
  internal -> size = 0; \
  internal -> chunk_size = chunk_size; \
  internal -> used = CMAP_T; \
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
static CMAP_SLIST_##NAME * name##_create(int chunk_size) \
{ \
  if(chunk_size <= 0) chunk_size = cmap_config_core_list_chunk_size(); \
 \
  CMAP_SLIST_##NAME * this = (CMAP_SLIST_##NAME *)CMAP_MEM_INSTANCE -> alloc( \
    sizeof(CMAP_SLIST_##NAME) + sizeof(NAME##_INTERNAL) + \
    sizeof(NAME##_CHUNK) + chunk_size * sizeof(type)); \
  name##_init(this, chunk_size); \
  return this; \
}

/*******************************************************************************
*******************************************************************************/

#define CMAP_SLIST_IMPL(NAME, name, type, dft) \
CMAP_SLIST_STATIC_FN_IMPL(NAME, name, type, dft) \
 \
const CMAP_SLIST_##NAME##_PUBLIC cmap_slist_##name##_public = { name##_create };

/*******************************************************************************
*******************************************************************************/

#define CMAP_SLIST_STATIC(NAME, name, type, dft) \
CMAP_SLIST_STRUCT_DECL(NAME, type) \
CMAP_SLIST_STATIC_FN_IMPL(NAME, name, type, dft)

/*******************************************************************************
*******************************************************************************/

#endif
