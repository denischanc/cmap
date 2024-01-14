#ifndef __CMAP_SET_DEFINE_H__
#define __CMAP_SET_DEFINE_H__

#include "cmap.h"
#include "cmap-tree.h"
#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

#define CMAP_SET_LOOP(macro) \
  macro(map, CMAP_MAP *) \
  macro(lc, CMAP_LIFECYCLE *)

/*******************************************************************************
*******************************************************************************/

#define CMAP_SET_STRUCT_DECL(name, type) \
typedef struct CMAP_SET_##name CMAP_SET_##name; \
 \
struct CMAP_SET_##name \
{ \
  CMAP_TREE_NODE node; \
 \
  type v; \
};

/*******************************************************************************
*******************************************************************************/

#define CMAP_SET_DECL(name, type) \
CMAP_SET_STRUCT_DECL(name, type) \
 \
typedef struct \
{ \
  char (*is)(CMAP_SET_##name * this, type v); \
  char (*add)(CMAP_SET_##name ** this, type v); \
  type (*rm)(CMAP_SET_##name ** this); \
  void (*clean)(CMAP_SET_##name ** this); \
} CMAP_SET_##name##_PUBLIC; \
 \
extern const CMAP_SET_##name##_PUBLIC cmap_set_##name##_public;

/*******************************************************************************
*******************************************************************************/

#define CMAP_SET_STATIC_FN_IMPL(name, type) \
static int CMAP_TREE_EVALFN_NAME(name)(void * node, void * data); \
 \
CMAP_TREE_RUNNER(name, CMAP_F, CMAP_F); \
 \
static char set_##name##_is(CMAP_SET_##name * this, type v) \
{ \
  CMAP_SET_##name data; \
  data.v = v; \
  return (CMAP_TREE_FINDFN(name, this, &data) != NULL); \
} \
 \
static char set_##name##_add(CMAP_SET_##name ** this, type v) \
{ \
  if(!set_##name##_is(*this, v)) \
  { \
    CMAP_KERNEL_ALLOC_PTR(node, CMAP_SET_##name); \
    node -> v = v; \
    CMAP_TREE_ADDFN(name, this, node, node); \
    return CMAP_T; \
  } \
  return CMAP_F; \
} \
 \
static type set_##name##_rm(CMAP_SET_##name ** this) \
{ \
  CMAP_SET_##name * node_ret = *this; \
  CMAP_TREE_RMFN(name, this, node_ret); \
 \
  type ret = node_ret -> v; \
  CMAP_KERNEL_FREE(node_ret); \
  return ret; \
} \
 \
static void set_##name##_clean_node(void * node, void * data) \
{ \
  CMAP_KERNEL_FREE(node); \
} \
 \
static void set_##name##_clean(CMAP_SET_##name ** this) \
{ \
  CMAP_TREE_CLEANFN(name, this, set_##name##_clean_node, NULL); \
}

/*******************************************************************************
*******************************************************************************/

#define CMAP_SET_IMPL(name, type) \
CMAP_SET_STATIC_FN_IMPL(name, type) \
 \
const CMAP_SET_##name##_PUBLIC cmap_set_##name##_public = \
{ \
  set_##name##_is, \
  set_##name##_add, \
  set_##name##_rm, \
  set_##name##_clean \
};

/*******************************************************************************
*******************************************************************************/

#define CMAP_SET_STATIC(name, type) \
CMAP_SET_STRUCT_DECL(name, type) \
CMAP_SET_STATIC_FN_IMPL(name, type)

/*******************************************************************************
*******************************************************************************/

#endif
