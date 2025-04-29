#ifndef __CMAP_SSET_DEFINE_H__
#define __CMAP_SSET_DEFINE_H__

#include "cmap.h"
#include "cmap-stree.h"
#include "cmap-kernel.h"
#include "cmap-map-type.h"
#include "cmap-lifecycle-type.h"

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_LOOP(macro) \
  macro(MAP, map, CMAP_MAP *, map_eval, map_log_v) \
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
typedef void (*CMAP_SSET_APPLY_FN_##NAME)(type * v, void * data);

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
static inline int eval_fn(type v_l, type v_r); \
 \
static int name##_stree_eval(void * node, void * data) \
{ \
  return eval_fn(((CMAP_SSET_##NAME *)node) -> v, \
    ((CMAP_SSET_##NAME *)data) -> v); \
} \
 \
static inline const char * log_fn(type v); \
 \
static const char * name##_stree_log(void * node) \
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
  CMAP_SSET_##NAME data; \
  data.v = v; \
  return CMAP_STREE_FINDFN(name, this, &data); \
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
  CMAP_KERNEL_ALLOC_PTR(node, CMAP_SSET_##NAME); \
  node -> v = v; \
  CMAP_STREE_ADDFN(name, this, node, node); \
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
  CMAP_STREE_RMFN(name, this, node_ret); \
 \
  type ret = node_ret -> v; \
  CMAP_KERNEL_FREE(node_ret); \
  return ret; \
} \
 \
CMAP_UNUSED static char name##_rm_v(CMAP_SSET_##NAME ** this, type v) \
{ \
  CMAP_SSET_##NAME * to_rm = name##_find(*this, v); \
  if(to_rm == NULL) return CMAP_F; \
 \
  CMAP_STREE_RMFN(name, this, to_rm); \
  CMAP_KERNEL_FREE(to_rm); \
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
static void name##_apply_apply(void * node, char is_eq, void * data) \
{ \
  CMAP_SSET_##NAME * node_ = node; \
  NAME##_APPLY_APPLY_DATA * data_ = data; \
  data_ -> fn(&node_ -> v, data_ -> data); \
} \
 \
CMAP_UNUSED static void name##_apply(CMAP_SSET_##NAME * this, \
  CMAP_SSET_APPLY_FN_##NAME fn, void * data) \
{ \
  NAME##_APPLY_APPLY_DATA data_ = {fn, data}; \
  CMAP_STREE_QUICKAPPLYFN(name, this, name##_apply_apply, &data_); \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
static void name##_clean_apply(void * node, char is_eq, void * data) \
{ \
  CMAP_KERNEL_FREE(node); \
} \
 \
CMAP_UNUSED static void name##_clean(CMAP_SSET_##NAME ** this) \
{ \
  CMAP_STREE_QUICKAPPLYFN(name, *this, name##_clean_apply, NULL); \
  *this = NULL; \
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
