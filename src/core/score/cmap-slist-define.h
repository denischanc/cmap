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

#define CMAP_SLIST_DECL(NAME, name, type, dft) \
typedef struct CMAP_SLIST_##NAME CMAP_SLIST_##NAME; \
 \
typedef void (*CMAP_SLIST_##NAME##_APPLY_FN)(type * v, void * data); \
 \
void cmap_slist_##name##_push(CMAP_SLIST_##NAME * list, type v); \
type cmap_slist_##name##_pop(CMAP_SLIST_##NAME * list); \
 \
void cmap_slist_##name##_unshift(CMAP_SLIST_##NAME * list, type v); \
type cmap_slist_##name##_shift(CMAP_SLIST_##NAME * list); \
 \
char cmap_slist_##name##_add(CMAP_SLIST_##NAME * list, int i, type v); \
type cmap_slist_##name##_rm(CMAP_SLIST_##NAME * list, int i); \
 \
type * cmap_slist_##name##_get(CMAP_SLIST_##NAME * list, int i); \
type * cmap_slist_##name##_first(CMAP_SLIST_##NAME * list); \
type * cmap_slist_##name##_last(CMAP_SLIST_##NAME * list); \
 \
int cmap_slist_##name##_size(CMAP_SLIST_##NAME * list); \
 \
void cmap_slist_##name##_apply(CMAP_SLIST_##NAME * list, \
  CMAP_SLIST_##NAME##_APPLY_FN fn, void * data); \
 \
void cmap_slist_##name##_clean(CMAP_SLIST_##NAME * list); \
 \
void cmap_slist_##name##_delete(CMAP_SLIST_##NAME * list); \
CMAP_SLIST_##NAME * cmap_slist_##name##_create(int chunk_size);

/*******************************************************************************
*******************************************************************************/

#define CMAP_SLIST_IMPL(NAME, name, type, dft) \
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
struct CMAP_SLIST_##NAME \
{ \
  NAME##_CHUNK * first, * last; \
  int size, chunk_size; \
  char used; \
}; \
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
static NAME##_CHUNK * name##_create_chunk(CMAP_SLIST_##NAME * list) \
{ \
  NAME##_CHUNK * chunk; \
  type * first; \
  if(list -> used) \
  { \
    int chunk_size = list -> chunk_size; \
    chunk = cmap_mem_alloc(sizeof(NAME##_CHUNK) + chunk_size * sizeof(type)); \
    first = (type *)(chunk + 1); \
    chunk -> first = first; \
    chunk -> last = first + (chunk_size - 1); \
  } \
  else \
  { \
    list -> used = CMAP_T; \
    chunk = (NAME##_CHUNK *)(list + 1); \
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
static void name##_create_first(CMAP_SLIST_##NAME * list) \
{ \
  NAME##_CHUNK * chunk = name##_create_chunk(list); \
  chunk -> next = list -> first; \
  list -> first -> prev = chunk; \
  list -> first = chunk; \
} \
 \
static void name##_delete_first(CMAP_SLIST_##NAME * list) \
{ \
  NAME##_CHUNK * chunk = list -> first; \
  list -> first = chunk -> next; \
  list -> first -> prev = NULL; \
  if(chunk == (NAME##_CHUNK *)(list + 1)) list -> used = CMAP_F; \
  else cmap_mem_free(chunk); \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_create_last(CMAP_SLIST_##NAME * list) \
{ \
  NAME##_CHUNK * chunk = name##_create_chunk(list); \
  chunk -> prev = list -> last; \
  list -> last -> next = chunk; \
  list -> last = chunk; \
} \
 \
static void name##_delete_last(CMAP_SLIST_##NAME * list) \
{ \
  NAME##_CHUNK * chunk = list -> last; \
  list -> last = chunk -> prev; \
  list -> last -> next = NULL; \
  if(chunk == (NAME##_CHUNK *)(list + 1)) list -> used = CMAP_F; \
  else cmap_mem_free(chunk); \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
void cmap_slist_##name##_push(CMAP_SLIST_##NAME * list, type v) \
{ \
  NAME##_CHUNK * last = list -> last; \
  if(last -> size >= list -> chunk_size) \
  { \
    name##_create_last(list); \
    last = list -> last; \
  } \
 \
  *last -> stop = v; \
 \
  name##_inc_w_last(&last -> stop, last); \
  last -> size++; \
  list -> size++; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
type cmap_slist_##name##_pop(CMAP_SLIST_##NAME * list) \
{ \
  NAME##_CHUNK * last = list -> last; \
  if(last -> size <= 0) return dft; \
  else \
  { \
    name##_dec_w_last(&last -> stop, last); \
    type ret = *last -> stop; \
 \
    if((last -> size <= 1) && (last != list -> first)) \
      name##_delete_last(list); \
    else last -> size--; \
    list -> size--; \
 \
    return ret; \
  } \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
void cmap_slist_##name##_unshift(CMAP_SLIST_##NAME * list, type v) \
{ \
  NAME##_CHUNK * first = list -> first; \
  if(first -> size >= list -> chunk_size) \
  { \
    name##_create_first(list); \
    first = list -> first; \
  } \
 \
  name##_dec_w_last(&first -> start, first); \
  first -> size++; \
  list -> size++; \
 \
  *first -> start = v; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
type cmap_slist_##name##_shift(CMAP_SLIST_##NAME * list) \
{ \
  NAME##_CHUNK * first = list -> first; \
  if(first -> size <= 0) return dft; \
  else \
  { \
    type ret = *first -> start; \
 \
    if((first -> size <= 1) && (first != list -> last)) \
      name##_delete_first(list); \
    else \
    { \
      name##_inc_w_last(&first -> start, first); \
      first -> size--; \
    } \
    list -> size--; \
 \
    return ret; \
  } \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static NAME##_CHUNK * name##_add_begin_get_chunk(CMAP_SLIST_##NAME * list, \
  int * i) \
{ \
  NAME##_CHUNK * chunk = list -> first; \
  if(chunk -> size >= list -> chunk_size) \
  { \
    name##_create_first(list); \
    chunk = list -> first; \
  } \
 \
  chunk -> size++; \
  list -> size++; \
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
static void name##_add_begin(CMAP_SLIST_##NAME * list, int i, type v) \
{ \
  NAME##_CHUNK * chunk = name##_add_begin_get_chunk(list, &i); \
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
static NAME##_CHUNK * name##_add_end_get_chunk(CMAP_SLIST_##NAME * list, \
  int * i) \
{ \
  NAME##_CHUNK * chunk = list -> last; \
  if(chunk -> size >= list -> chunk_size) \
  { \
    name##_create_last(list); \
    chunk = list -> last; \
  } \
 \
  chunk -> size++; \
  list -> size++; \
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
static void name##_add_end(CMAP_SLIST_##NAME * list, int i, type v) \
{ \
  NAME##_CHUNK * chunk = name##_add_end_get_chunk(list, &i); \
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
char cmap_slist_##name##_add(CMAP_SLIST_##NAME * list, int i, type v) \
{ \
  if((i >= 0) && (i <= list -> size)) \
  { \
    if(i <= (list -> size >> 1)) name##_add_begin(list, i, v); \
    else name##_add_end(list, list -> size - i, v); \
    return CMAP_T; \
  } \
  return CMAP_F; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_rm_begin_upd_chunk(CMAP_SLIST_##NAME * list, \
  NAME##_CHUNK * chunk) \
{ \
  NAME##_CHUNK * first = list -> first; \
  while(chunk != first) \
  { \
    name##_dec_w_last(&chunk -> start, chunk); \
    NAME##_CHUNK * prev_chunk = chunk -> prev; \
    name##_dec_w_last(&prev_chunk -> stop, prev_chunk); \
    *chunk -> start = *prev_chunk -> stop; \
    chunk = prev_chunk; \
  } \
 \
  if((first -> size <= 1) && (first != list -> last)) \
    name##_delete_first(list); \
  else first -> size--; \
  list -> size--; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static NAME##_CHUNK * name##_rm_begin_get_chunk(CMAP_SLIST_##NAME * list, \
  int * i) \
{ \
  NAME##_CHUNK * chunk = list -> first; \
  while(*i >= chunk -> size) \
  { \
    (*i) -= chunk -> size; \
    chunk = chunk -> next; \
  } \
  return chunk; \
} \
 \
static type name##_rm_begin(CMAP_SLIST_##NAME * list, int i) \
{ \
  NAME##_CHUNK * chunk = name##_rm_begin_get_chunk(list, &i); \
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
  name##_rm_begin_upd_chunk(list, chunk); \
 \
  return v; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_rm_end_upd_chunk(CMAP_SLIST_##NAME * list, \
  NAME##_CHUNK * chunk) \
{ \
  NAME##_CHUNK * last = list -> last; \
  while(chunk != last) \
  { \
    NAME##_CHUNK * next_chunk = chunk -> next; \
    *chunk -> stop = *next_chunk -> start; \
    name##_inc_w_last(&next_chunk -> start, next_chunk); \
    name##_inc_w_last(&chunk -> stop, chunk); \
    chunk = next_chunk; \
  } \
 \
  if((last -> size <= 1) && (last != list -> first)) \
    name##_delete_last(list); \
  else last -> size--; \
  list -> size--; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static NAME##_CHUNK * name##_rm_end_get_chunk(CMAP_SLIST_##NAME * list, \
  int * i) \
{ \
  NAME##_CHUNK * chunk = list -> last; \
  while(*i >= chunk -> size) \
  { \
    (*i) -= chunk -> size; \
    chunk = chunk -> prev; \
  } \
  return chunk; \
} \
 \
static type name##_rm_end(CMAP_SLIST_##NAME * list, int i) \
{ \
  NAME##_CHUNK * chunk = name##_rm_end_get_chunk(list, &i); \
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
  name##_rm_end_upd_chunk(list, chunk); \
 \
  return v; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
type cmap_slist_##name##_rm(CMAP_SLIST_##NAME * list, int i) \
{ \
  if((i < 0) || (i >= list -> size)) return dft; \
  else \
  { \
    if(i < (list -> size >> 1)) return name##_rm_begin(list, i); \
    else return name##_rm_end(list, list -> size - 1 - i); \
  } \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static NAME##_CHUNK * name##_get_chunk(CMAP_SLIST_##NAME * list, int * i) \
{ \
  NAME##_CHUNK * chunk = list -> first; \
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
type * cmap_slist_##name##_get(CMAP_SLIST_##NAME * list, int i) \
{ \
  NAME##_CHUNK * chunk = name##_get_chunk(list, &i); \
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
type * cmap_slist_##name##_first(CMAP_SLIST_##NAME * list) \
{ \
  NAME##_CHUNK * first = list -> first; \
  if(first -> size <= 0) return NULL; \
  else return first -> start; \
} \
 \
type * cmap_slist_##name##_last(CMAP_SLIST_##NAME * list) \
{ \
  NAME##_CHUNK * last = list -> last; \
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
int cmap_slist_##name##_size(CMAP_SLIST_##NAME * list) \
{ \
  return list -> size; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
void cmap_slist_##name##_apply(CMAP_SLIST_##NAME * list, \
  CMAP_SLIST_##NAME##_APPLY_FN fn, void * data) \
{ \
  NAME##_CHUNK * chunk = list -> first; \
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
void cmap_slist_##name##_clean(CMAP_SLIST_##NAME * list) \
{ \
  while(list -> first != list -> last) name##_delete_last(list); \
  NAME##_CHUNK * first = list -> first; \
  type * first_first = first -> first; \
  first -> start = first_first; \
  first -> stop = first_first; \
  first -> size = 0; \
  list -> size = 0; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_delete_chunks(CMAP_SLIST_##NAME * list) \
{ \
  NAME##_CHUNK * no_free = (NAME##_CHUNK *)(list + 1), \
    * chunk = list -> first; \
  while(chunk != NULL) \
  { \
    NAME##_CHUNK * tmp = chunk; \
    chunk = chunk -> next; \
    if(tmp != no_free) cmap_mem_free(tmp); \
  } \
} \
 \
void cmap_slist_##name##_delete(CMAP_SLIST_##NAME * list) \
{ \
  name##_delete_chunks(list); \
  cmap_mem_free(list); \
} \
 \
static void name##_init(CMAP_SLIST_##NAME * list, int chunk_size) \
{ \
  NAME##_CHUNK * first = (NAME##_CHUNK *)(list + 1); \
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
  list -> first = first; \
  list -> last = first; \
  list -> size = 0; \
  list -> chunk_size = chunk_size; \
  list -> used = CMAP_T; \
} \
 \
CMAP_SLIST_##NAME * cmap_slist_##name##_create(int chunk_size) \
{ \
  if(chunk_size <= 0) chunk_size = cmap_config_core_list_chunk_size(); \
 \
  CMAP_SLIST_##NAME * list = cmap_mem_alloc(sizeof(CMAP_SLIST_##NAME) + \
    sizeof(NAME##_CHUNK) + chunk_size * sizeof(type)); \
  name##_init(list, chunk_size); \
  return list; \
}

/*******************************************************************************
*******************************************************************************/

#define CMAP_SLIST(NAME, name, type, dft) \
  CMAP_SLIST_DECL(NAME, name, type, dft) \
  CMAP_SLIST_IMPL(NAME, name, type, dft)

/*******************************************************************************
*******************************************************************************/

#endif
