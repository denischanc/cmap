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
  macro(map, CMAP_MAP *) \
  macro(lc, CMAP_LIFECYCLE *)

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

#define CMAP_SSET_DECL(name, type) \
CMAP_SSET_STRUCT_DECL(name, type) \
 \
typedef struct \
{ \
  char (*is)(CMAP_SSET_##name * this, type v); \
  char (*add)(CMAP_SSET_##name ** this, type v); \
  type (*rm)(CMAP_SSET_##name ** this); \
  void (*clean)(CMAP_SSET_##name ** this); \
  void (*log)(CMAP_SSET_##name * this, char lvl); \
} CMAP_SSET_##name##_PUBLIC; \
 \
extern const CMAP_SSET_##name##_PUBLIC cmap_sset_##name##_public;

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_STATIC_FN_IMPL(name, type) \
static int CMAP_STREE_EVALFN_NAME(name)(void * node, void * data); \
 \
CMAP_STREE_RUNNER(name, CMAP_F, CMAP_F); \
 \
static char sset_##name##_is(CMAP_SSET_##name * this, type v) \
{ \
  CMAP_SSET_##name data; \
  data.v = v; \
  return (CMAP_STREE_FINDFN(name, this, &data) != NULL); \
} \
 \
static char sset_##name##_add(CMAP_SSET_##name ** this, type v) \
{ \
  if(!sset_##name##_is(*this, v)) \
  { \
    CMAP_KERNEL_ALLOC_PTR(node, CMAP_SSET_##name); \
    node -> v = v; \
    CMAP_STREE_ADDFN(name, this, node, node); \
    return CMAP_T; \
  } \
  return CMAP_F; \
} \
 \
static type sset_##name##_rm(CMAP_SSET_##name ** this) \
{ \
  CMAP_SSET_##name * node_ret = *this; \
  CMAP_STREE_RMFN(name, this, node_ret); \
 \
  type ret = node_ret -> v; \
  CMAP_KERNEL_FREE(node_ret); \
  return ret; \
} \
 \
static void sset_##name##_clean_node(void * node, void * data) \
{ \
  CMAP_KERNEL_FREE(node); \
} \
 \
static void sset_##name##_clean(CMAP_SSET_##name ** this) \
{ \
  CMAP_STREE_CLEANFN(name, this, sset_##name##_clean_node, NULL); \
} \
 \
static void * sset_##name##_log_ptr(void * node) \
{ \
  return (void *)((CMAP_SSET_##name *)node) -> v; \
} \
 \
static void sset_##name##_log(CMAP_SSET_##name * this, char lvl) \
{ \
  cmap_stree_public.log(lvl, &CMAP_STREE_RUNNER_NAME(name), this, \
    sset_##name##_log_ptr); \
}

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_IMPL(name, type) \
CMAP_SSET_STATIC_FN_IMPL(name, type) \
 \
const CMAP_SSET_##name##_PUBLIC cmap_sset_##name##_public = \
{ \
  sset_##name##_is, \
  sset_##name##_add, \
  sset_##name##_rm, \
  sset_##name##_clean, \
  sset_##name##_log \
};

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_STATIC(name, type) \
CMAP_SSET_STRUCT_DECL(name, type) \
CMAP_SSET_STATIC_FN_IMPL(name, type)

/*******************************************************************************
*******************************************************************************/

#endif
