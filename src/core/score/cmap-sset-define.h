#ifndef __CMAP_SSET_DEFINE_H__
#define __CMAP_SSET_DEFINE_H__

#include "cmap.h"
#include "cmap-stree.h"
#include "cmap-mem.h"
#include "cmap-map-type.h"
#include "cmap-lifecycle-type.h"
#include "cmap-iterator-define.h"
#include "cmap-stree-iterator.h"

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_LOOP(macro) \
  macro(LC, lc, CMAP_LIFECYCLE *, lc_eval, lc_log_v)

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_STRUCT_DECL(NAME, type) \
typedef struct \
{ \
  CMAP_STREE_NODE node; \
 \
  type v; \
} CMAP_SSET_##NAME; \
 \
typedef void (*CMAP_SSET_APPLY_FN_##NAME)(type * v, void * data); \
 \
CMAP_ITERATOR_DECL(SSET_##NAME, type *)

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_DECL(NAME, name, type, eval_fn, log_fn) \
CMAP_SSET_STRUCT_DECL(NAME, type) \
 \
typedef struct \
{ \
  char (*is)(CMAP_SSET_##NAME * this, type v); \
  type * (*get)(CMAP_SSET_##NAME * this, type v); \
  void (*add_force)(CMAP_SSET_##NAME ** this, type v); \
  char (*add)(CMAP_SSET_##NAME ** this, type v); \
  type (*rm)(CMAP_SSET_##NAME ** this); \
  char (*rm_v)(CMAP_SSET_##NAME ** this, type v); \
  int (*size)(CMAP_SSET_##NAME * this); \
  void (*apply)(CMAP_SSET_##NAME * this, CMAP_SSET_APPLY_FN_##NAME fn, \
    void * data); \
  void (*clean)(CMAP_SSET_##NAME ** this); \
  CMAP_ITERATOR_SSET_##NAME * (*iterator)(CMAP_SSET_##NAME ** this); \
  void (*log)(CMAP_SSET_##NAME * this, char lvl); \
} CMAP_SSET_##NAME##_PUBLIC; \
 \
extern const CMAP_SSET_##NAME##_PUBLIC cmap_sset_##name##_public;

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_STATIC_FN_IMPL(NAME, name, type, eval_fn, log_fn) \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static inline int64_t eval_fn(type v_l, type v_r); \
 \
static int64_t name##_stree_eval(CMAP_STREE_NODE * node, void * data) \
{ \
  return eval_fn(((CMAP_SSET_##NAME *)node) -> v, *(type *)data); \
} \
 \
static inline const char * log_fn(type v); \
 \
static const char * name##_stree_log(CMAP_STREE_NODE * node) \
{ \
  return log_fn(((CMAP_SSET_##NAME *)node) -> v); \
} \
 \
CMAP_STREE_RUNNER(name, name##_stree_eval, name##_stree_log, CMAP_F, CMAP_F); \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static CMAP_SSET_##NAME * name##_find(CMAP_SSET_##NAME * this, type v) \
{ \
  return (CMAP_SSET_##NAME *)CMAP_STREE_FINDFN(name, this, &v); \
} \
 \
static char name##_is(CMAP_SSET_##NAME * this, type v) \
{ \
  return (name##_find(this, v) != NULL); \
} \
 \
CMAP_UNUSED static type * name##_get(CMAP_SSET_##NAME * this, type v) \
{ \
  CMAP_SSET_##NAME * node = name##_find(this, v); \
  return (node == NULL) ? NULL : &node -> v; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_add_force(CMAP_SSET_##NAME ** this, type v) \
{ \
  CMAP_MEM_ALLOC_PTR(node, CMAP_SSET_##NAME); \
  node -> v = v; \
  CMAP_STREE_ADDFN(name, this, node, &v); \
} \
 \
CMAP_UNUSED static char name##_add(CMAP_SSET_##NAME ** this, type v) \
{ \
  if(!name##_is(*this, v)) \
  { \
    name##_add_force(this, v); \
    return CMAP_T; \
  } \
  return CMAP_F; \
} \
 \
CMAP_UNUSED static type name##_rm(CMAP_SSET_##NAME ** this) \
{ \
  CMAP_SSET_##NAME * node_ret = *this; \
  CMAP_STREE_RMFN(this, node_ret); \
 \
  type ret = node_ret -> v; \
  cmap_mem_free(node_ret); \
  return ret; \
} \
 \
CMAP_UNUSED static char name##_rm_v(CMAP_SSET_##NAME ** this, type v) \
{ \
  CMAP_SSET_##NAME * to_rm = name##_find(*this, v); \
  if(to_rm == NULL) return CMAP_F; \
 \
  CMAP_STREE_RMFN(this, to_rm); \
  cmap_mem_free(to_rm); \
  return CMAP_T; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
CMAP_UNUSED static int name##_size(CMAP_SSET_##NAME * this) \
{ \
  return this -> node.size; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
typedef struct \
{ \
  CMAP_SSET_APPLY_FN_##NAME fn; \
  void * data; \
} NAME##_APPLY_APPLY_DATA; \
 \
static void name##_apply_apply(CMAP_STREE_NODE * node, char is_eq, void * data) \
{ \
  CMAP_SSET_##NAME * node_ = (CMAP_SSET_##NAME *)node; \
  NAME##_APPLY_APPLY_DATA * data_ = data; \
  data_ -> fn(&node_ -> v, data_ -> data); \
} \
 \
CMAP_UNUSED static void name##_apply(CMAP_SSET_##NAME * this, \
  CMAP_SSET_APPLY_FN_##NAME fn, void * data) \
{ \
  NAME##_APPLY_APPLY_DATA data_ = {fn, data}; \
  CMAP_STREE_QUICKAPPLYFN(this, name##_apply_apply, &data_); \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_clean_apply(CMAP_STREE_NODE * node, char is_eq, \
  void * data) \
{ \
  cmap_mem_free(node); \
} \
 \
CMAP_UNUSED static void name##_clean(CMAP_SSET_##NAME ** this) \
{ \
  CMAP_STREE_QUICKAPPLYFN(*this, name##_clean_apply, NULL); \
  *this = NULL; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static type * name##_it_next(CMAP_ITERATOR_SSET_##NAME * this) \
{ \
  CMAP_STREE_NODE * node = cmap_stree_iterator_public.next( \
    (CMAP_ITERATOR_STREE *)this); \
  return (node == NULL) ? NULL : &((CMAP_SSET_##NAME *)node) -> v; \
} \
 \
static type * name##_it_get(CMAP_ITERATOR_SSET_##NAME * this) \
{ \
  CMAP_STREE_NODE * node = cmap_stree_iterator_public.get( \
    (CMAP_ITERATOR_STREE *)this); \
  return (node == NULL) ? NULL : &((CMAP_SSET_##NAME *)node) -> v; \
} \
 \
static void name##_it_rm(CMAP_ITERATOR_SSET_##NAME * this) \
{ \
  CMAP_STREE_NODE * node = cmap_stree_iterator_public.get( \
    (CMAP_ITERATOR_STREE *)this); \
  cmap_stree_iterator_public.rm((CMAP_ITERATOR_STREE *)this); \
  cmap_mem_free(node); \
} \
 \
CMAP_UNUSED static CMAP_ITERATOR_SSET_##NAME * name##_iterator( \
  CMAP_SSET_##NAME ** this) \
{ \
  CMAP_ITERATOR_SSET_##NAME * it = (CMAP_ITERATOR_SSET_##NAME *) \
    cmap_stree_iterator_public.create((CMAP_STREE_NODE **)this); \
  it -> next = name##_it_next; \
  it -> get = name##_it_get; \
  it -> rm = name##_it_rm; \
  return it; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
CMAP_UNUSED static void name##_log(CMAP_SSET_##NAME * this, char lvl) \
{ \
  CMAP_STREE_LOGFN(name, lvl, this); \
}

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_IMPL(NAME, name, type, eval_fn, log_fn) \
CMAP_SSET_STATIC_FN_IMPL(NAME, name, type, eval_fn, log_fn) \
 \
const CMAP_SSET_##NAME##_PUBLIC cmap_sset_##name##_public = \
{ \
  name##_is, \
  name##_get, \
  name##_add_force, name##_add, \
  name##_rm, name##_rm_v, \
  name##_size, \
  name##_apply, \
  name##_clean, \
  name##_iterator, \
  name##_log \
};

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_STATIC(NAME, name, type, eval_fn, log_fn) \
CMAP_SSET_STRUCT_DECL(NAME, type) \
CMAP_SSET_STATIC_FN_IMPL(NAME, name, type, eval_fn, log_fn)

/*******************************************************************************
*******************************************************************************/

#endif
