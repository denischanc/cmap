
#include "cmap-sset.h"

/*******************************************************************************
*******************************************************************************/

#define CMAP_SSET_PTR_EVALFN_IMPL(name, type) \
static int CMAP_STREE_EVALFN_NAME(name)(void * node, void * data) \
{ \
  type name##_l = ((CMAP_SSET_##name *)node) -> v; \
  type name##_r = ((CMAP_SSET_##name *)data) -> v; \
 \
  if(name##_l > name##_r) return 1; \
  else if(name##_l < name##_r) return -1; \
  else return 0; \
}

/*******************************************************************************
*******************************************************************************/

CMAP_SSET_LOOP(CMAP_SSET_IMPL)

CMAP_SSET_PTR_EVALFN_IMPL(map, CMAP_MAP *)
CMAP_SSET_PTR_EVALFN_IMPL(lc, CMAP_LIFECYCLE *)
