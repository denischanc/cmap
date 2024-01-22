
#include "cmap-set.h"

/*******************************************************************************
*******************************************************************************/

#define CMAP_SET_PTR_EVALFN_IMPL(name, type) \
static int CMAP_STREE_EVALFN_NAME(name)(void * node, void * data) \
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

CMAP_SET_LOOP(CMAP_SET_IMPL)

CMAP_SET_PTR_EVALFN_IMPL(map, CMAP_MAP *)
CMAP_SET_PTR_EVALFN_IMPL(lc, CMAP_LIFECYCLE *)
