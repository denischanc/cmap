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
  macro(map, CMAP_MAP *, map_runner_log) \
  macro(lc, CMAP_LIFECYCLE *, lc_runner_log)

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_STRUCT_DECL(name, type) \
typedef struct CMAP_SSET_##name CMAP_SSET_##name; \
 \
struct CMAP_SSET_##name \
{ \
  CMAP_STREE_NODE node; \
 \
  type v; \
};

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_DECL(name, type, runner_log) \
CMAP_SSET_STRUCT_DECL(name, type) \
 \
typedef struct \
{ \
  char (*is)(CMAP_SSET_##name * this, type v); \
  type * (*get)(CMAP_SSET_##name * this, type v); \
  void (*add_force)(CMAP_SSET_##name ** this, type v); \
  char (*add)(CMAP_SSET_##name ** this, type v); \
  type (*rm)(CMAP_SSET_##name ** this); \
  char (*rm_v)(CMAP_SSET_##name ** this, type v); \
  void (*clean)(CMAP_SSET_##name ** this); \
  void (*log)(CMAP_SSET_##name * this, char lvl); \
} CMAP_SSET_##name##_PUBLIC; \
 \
extern const CMAP_SSET_##name##_PUBLIC cmap_sset_##name##_public;

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_STATIC_FN_IMPL(name, type, runner_log) \
CMAP_STREE_RUNNER(name, runner_log, CMAP_F, CMAP_F); \
 \
static CMAP_SSET_##name * name##_find(CMAP_SSET_##name * this, type v) \
{ \
  CMAP_SSET_##name data; \
  data.v = v; \
  return CMAP_STREE_FINDFN(name, this, &data); \
} \
 \
static char name##_is(CMAP_SSET_##name * this, type v) \
{ \
  return (name##_find(this, v) != NULL); \
} \
 \
CMAP_UNUSED static type * name##_get(CMAP_SSET_##name * this, type v) \
{ \
  return &name##_find(this, v) -> v; \
} \
 \
static void name##_add_force(CMAP_SSET_##name ** this, type v) \
{ \
  CMAP_KERNEL_ALLOC_PTR(node, CMAP_SSET_##name); \
  node -> v = v; \
  CMAP_STREE_ADDFN(name, this, node, node); \
} \
 \
CMAP_UNUSED static char name##_add(CMAP_SSET_##name ** this, type v) \
{ \
  if(!name##_is(*this, v)) \
  { \
    name##_add_force(this, v); \
    return CMAP_T; \
  } \
  return CMAP_F; \
} \
 \
CMAP_UNUSED static type name##_rm(CMAP_SSET_##name ** this) \
{ \
  CMAP_SSET_##name * node_ret = *this; \
  CMAP_STREE_RMFN(name, this, node_ret); \
 \
  type ret = node_ret -> v; \
  CMAP_KERNEL_FREE(node_ret); \
  return ret; \
} \
 \
CMAP_UNUSED static char name##_rm_v(CMAP_SSET_##name ** this, type v) \
{ \
  CMAP_SSET_##name * to_rm = name##_find(*this, v); \
  if(to_rm == NULL) return CMAP_F; \
 \
  CMAP_STREE_RMFN(name, this, to_rm); \
  return CMAP_T; \
} \
 \
static void name##_clean_apply(void * node, char is_eq, void * data) \
{ \
  CMAP_KERNEL_FREE(node); \
} \
 \
CMAP_UNUSED static void name##_clean(CMAP_SSET_##name ** this) \
{ \
  CMAP_STREE_QUICKAPPLYFN(name, *this, name##_clean_apply, NULL); \
  *this = NULL; \
} \
 \
CMAP_UNUSED static void name##_log(CMAP_SSET_##name * this, char lvl) \
{ \
  CMAP_STREE_LOGFN(name, lvl, this); \
}

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_IMPL(name, type, runner_log) \
CMAP_SSET_STATIC_FN_IMPL(name, type, runner_log) \
 \
const CMAP_SSET_##name##_PUBLIC cmap_sset_##name##_public = \
{ \
  name##_is, \
  name##_get, \
  name##_add_force, \
  name##_add, \
  name##_rm, \
  name##_rm_v, \
  name##_clean, \
  name##_log \
};

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_STATIC(name, type, runner_log) \
CMAP_SSET_STRUCT_DECL(name, type) \
CMAP_SSET_STATIC_FN_IMPL(name, type, runner_log)

/*******************************************************************************
*******************************************************************************/

#endif
