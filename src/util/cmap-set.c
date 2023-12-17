
#include "cmap-set.h"

/*******************************************************************************
*******************************************************************************/

#define CMAP_SET_EVALFN_IMPL(name, type) \
static int CMAP_TREE_EVALFN_NAME(name)(CMAP_TREE_RUNNER * this, \
  void * node, void * data) \
{ \
  type name##_l = ((CMAP_SET_##name *)node) -> v; \
  type name##_r = ((CMAP_SET_##name *)data) -> v; \
 \
  if(name##_l > name##_r) return 1; \
  else if(name##_l < name##_r) return -1; \
  else return 0; \
}

/*******************************************************************************
*******************************************************************************/

CMAP_SET_IMPL(map, CMAP_MAP *)
CMAP_SET_EVALFN_IMPL(map, CMAP_MAP *)

CMAP_SET_IMPL(lc, CMAP_LIFECYCLE *)
CMAP_SET_EVALFN_IMPL(lc, CMAP_LIFECYCLE *)
