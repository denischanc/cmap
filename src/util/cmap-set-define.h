#ifndef __CMAP_SET_DEFINE_H__
#define __CMAP_SET_DEFINE_H__

#include "cmap.h"
#include "cmap-tree.h"
#include "cmap-kernel.h"

#define CMAP_SET_DEF(name, type) \
typedef struct CMAP_SET_##name CMAP_SET_##name; \
 \
struct CMAP_SET_##name \
{ \
  CMAP_TREE_NODE node; \
 \
  type v; \
}; \
 \
CMAP_TREE_RUNNER(name, NULL, false, false); \
 \
static char cmap_set_##name##_add(CMAP_SET_##name ** set, type v) \
{ \
  CMAP_SET_##name data; \
  data.v = v; \
  if(CMAP_TREE_FINDFN(name, *set, &data) == NULL) \
  { \
    CMAP_KERNEL_ALLOC_PTR(node, CMAP_SET_##name); \
    node -> v = v; \
    CMAP_TREE_ADDFN(name, set, node, &data); \
    return CMAP_T; \
  } \
  return CMAP_F; \
} \
 \
static type cmap_set_##name##_rm(CMAP_SET_##name ** set) \
{ \
  CMAP_SET_##name * node_ret = *set; \
  CMAP_TREE_RMFN(name, set, node_ret); \
 \
  type ret = node_ret -> v; \
  CMAP_KERNEL_FREE(node_ret); \
  return ret; \
}

#endif
