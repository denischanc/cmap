#ifndef __CMAP_SSET_DEFINE_H__
#define __CMAP_SSET_DEFINE_H__

#include "cmap.h"
#include "cmap-stree.h"
#include "cmap-mem.h"
#include "cmap-map-type.h"
#include "cmap-lifecycle-type.h"
#include "cmap-iterator-define.h"

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_LOOP(macro) \
  macro(LC, lc, CMAP_LIFECYCLE *, lc_eval, lc_log_v)

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_DECL(NAME, name, type, eval_fn, log_fn) \
typedef struct CMAP_SSET_##NAME CMAP_SSET_##NAME; \
 \
typedef void (*CMAP_SSET_##NAME##_APPLY_FN)(type * v, void * data); \
 \
char cmap_sset_##name##_is(CMAP_SSET_##NAME * set, type v); \
type * cmap_sset_##name##_get(CMAP_SSET_##NAME * set, type v); \
  \
void cmap_sset_##name##_add_force(CMAP_SSET_##NAME ** set, type v); \
char cmap_sset_##name##_add(CMAP_SSET_##NAME ** set, type v); \
type cmap_sset_##name##_rm(CMAP_SSET_##NAME ** set); \
char cmap_sset_##name##_rm_v(CMAP_SSET_##NAME ** set, type v); \
 \
int cmap_sset_##name##_size(CMAP_SSET_##NAME * set); \
 \
void cmap_sset_##name##_apply(CMAP_SSET_##NAME * set, \
  CMAP_SSET_##NAME##_APPLY_FN fn, void * data); \
void cmap_sset_##name##_clean(CMAP_SSET_##NAME ** set); \
void cmap_sset_##name##_log(CMAP_SSET_##NAME * set, char lvl); \
 \
CMAP_ITERATOR_DECL(SSET_##NAME, sset_##name, type *) \
CMAP_ITERATOR_SSET_##NAME * cmap_iterator_sset_##name##_create( \
  CMAP_SSET_##NAME ** set);

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_IMPL(NAME, name, type, eval_fn, log_fn) \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
struct CMAP_SSET_##NAME \
{ \
  CMAP_STREE_NODE node; \
 \
  type v; \
}; \
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
static CMAP_SSET_##NAME * name##_find(CMAP_SSET_##NAME * set, type v) \
{ \
  return (CMAP_SSET_##NAME *)CMAP_STREE_FINDFN(name, set, &v); \
} \
 \
char cmap_sset_##name##_is(CMAP_SSET_##NAME * set, type v) \
{ \
  return (name##_find(set, v) != NULL); \
} \
 \
type * cmap_sset_##name##_get(CMAP_SSET_##NAME * set, type v) \
{ \
  CMAP_SSET_##NAME * node = name##_find(set, v); \
  return (node == NULL) ? NULL : &node -> v; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
void cmap_sset_##name##_add_force(CMAP_SSET_##NAME ** set, type v) \
{ \
  CMAP_MEM_ALLOC_PTR(node, CMAP_SSET_##NAME); \
  node -> v = v; \
  CMAP_STREE_ADDFN(name, set, node, &v); \
} \
 \
char cmap_sset_##name##_add(CMAP_SSET_##NAME ** set, type v) \
{ \
  if(!cmap_sset_##name##_is(*set, v)) \
  { \
    cmap_sset_##name##_add_force(set, v); \
    return CMAP_T; \
  } \
  return CMAP_F; \
} \
 \
type cmap_sset_##name##_rm(CMAP_SSET_##NAME ** set) \
{ \
  CMAP_SSET_##NAME * node_ret = *set; \
  CMAP_STREE_RMFN(set, node_ret); \
 \
  type ret = node_ret -> v; \
  cmap_mem_free(node_ret); \
  return ret; \
} \
 \
char cmap_sset_##name##_rm_v(CMAP_SSET_##NAME ** set, type v) \
{ \
  CMAP_SSET_##NAME * to_rm = name##_find(*set, v); \
  if(to_rm == NULL) return CMAP_F; \
 \
  CMAP_STREE_RMFN(set, to_rm); \
  cmap_mem_free(to_rm); \
  return CMAP_T; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
int cmap_sset_##name##_size(CMAP_SSET_##NAME * set) \
{ \
  return set -> node.size; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
typedef struct \
{ \
  CMAP_SSET_##NAME##_APPLY_FN fn; \
  void * data; \
} NAME##_APPLY_APPLY_DATA; \
 \
static void name##_apply_apply(CMAP_STREE_NODE * node, char is_eq, \
  void * data) \
{ \
  CMAP_SSET_##NAME * node_ = (CMAP_SSET_##NAME *)node; \
  NAME##_APPLY_APPLY_DATA * data_ = data; \
  data_ -> fn(&node_ -> v, data_ -> data); \
} \
 \
void cmap_sset_##name##_apply(CMAP_SSET_##NAME * set, \
  CMAP_SSET_##NAME##_APPLY_FN fn, void * data) \
{ \
  NAME##_APPLY_APPLY_DATA data_ = {fn, data}; \
  CMAP_STREE_QUICKAPPLYFN(set, name##_apply_apply, &data_); \
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
void cmap_sset_##name##_clean(CMAP_SSET_##NAME ** set) \
{ \
  CMAP_STREE_QUICKAPPLYFN(*set, name##_clean_apply, NULL); \
  *set = NULL; \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
void cmap_sset_##name##_log(CMAP_SSET_##NAME * set, char lvl) \
{ \
  CMAP_STREE_LOGFN(name, lvl, set); \
} \
 \
/***************************************************************************** \
*****************************************************************************/ \
 \
char cmap_iterator_sset_##name##_has_next(CMAP_ITERATOR_SSET_##NAME * it) \
{ \
  return cmap_iterator_stree_has_next((CMAP_ITERATOR_STREE *)it); \
} \
 \
type * cmap_iterator_sset_##name##_next(CMAP_ITERATOR_SSET_##NAME * it) \
{ \
  CMAP_STREE_NODE * node = cmap_iterator_stree_next( \
    (CMAP_ITERATOR_STREE *)it); \
  return (node == NULL) ? NULL : &((CMAP_SSET_##NAME *)node) -> v; \
} \
 \
type * cmap_iterator_sset_##name##_get(CMAP_ITERATOR_SSET_##NAME * it) \
{ \
  CMAP_STREE_NODE * node = cmap_iterator_stree_get( \
    (CMAP_ITERATOR_STREE *)it); \
  return (node == NULL) ? NULL : &((CMAP_SSET_##NAME *)node) -> v; \
} \
 \
void cmap_iterator_sset_##name##_rm(CMAP_ITERATOR_SSET_##NAME * it) \
{ \
  CMAP_STREE_NODE * node = cmap_iterator_stree_get( \
    (CMAP_ITERATOR_STREE *)it); \
  cmap_iterator_stree_rm((CMAP_ITERATOR_STREE *)it); \
  cmap_mem_free(node); \
} \
 \
void cmap_iterator_sset_##name##_delete(CMAP_ITERATOR_SSET_##NAME * it) \
{ \
  cmap_iterator_stree_delete((CMAP_ITERATOR_STREE *)it); \
} \
 \
CMAP_ITERATOR_SSET_##NAME * cmap_iterator_sset_##name##_create( \
  CMAP_SSET_##NAME ** set) \
{ \
  return (CMAP_ITERATOR_SSET_##NAME *)cmap_iterator_stree_create( \
    (CMAP_STREE_NODE **)set); \
}

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET(NAME, name, type, eval_fn, log_fn) \
  CMAP_SSET_DECL(NAME, name, type, eval_fn, log_fn) \
  CMAP_SSET_IMPL(NAME, name, type, eval_fn, log_fn)

/*******************************************************************************
*******************************************************************************/

#endif
