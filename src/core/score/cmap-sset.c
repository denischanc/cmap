
#include "cmap-sset.h"

/*******************************************************************************
*******************************************************************************/

#define PTR_LOOP(macro) \
  macro(MAP, map, CMAP_MAP *) \
  macro(LC, lc, CMAP_LIFECYCLE *)

/*******************************************************************************
*******************************************************************************/

#define PTR_EVALFN_IMPL(NAME, name, type) \
static int CMAP_STREE_EVALFN_NAME(name)(void * node, void * data) \
{ \
  type v_l = ((CMAP_SSET_##NAME *)node) -> v; \
  type v_r = ((CMAP_SSET_##NAME *)data) -> v; \
 \
  if(v_l > v_r) return 1; \
  else if(v_l < v_r) return -1; \
  else return 0; \
}

/*******************************************************************************
*******************************************************************************/

#define PTR_LOGFN_IMPL(NAME, name, type) \
static const char * name##_runner_log(void * node) \
{ \
  static char buffer[20]; \
  snprintf(buffer, sizeof(buffer), "%p", ((CMAP_SSET_##NAME *)node) -> v); \
  return buffer; \
}

/*******************************************************************************
*******************************************************************************/

PTR_LOOP(PTR_EVALFN_IMPL)
PTR_LOOP(PTR_LOGFN_IMPL)

CMAP_SSET_LOOP(CMAP_SSET_IMPL)
