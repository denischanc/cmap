
#include "cmap-sset.h"

/*******************************************************************************
*******************************************************************************/

#define PTR_LOOP(macro) \
  macro(map, CMAP_MAP *) \
  macro(lc, CMAP_LIFECYCLE *)

/*******************************************************************************
*******************************************************************************/

#define PTR_EVALFN_IMPL(name, type) \
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

#define PTR_LOGFN_IMPL(name, type) \
static const char * name##_runner_log(void * node) \
{ \
  static char buffer[20]; \
  snprintf(buffer, sizeof(buffer), "%p", ((CMAP_SSET_##name *)node) -> v); \
  return buffer; \
}

/*******************************************************************************
*******************************************************************************/

PTR_LOOP(PTR_EVALFN_IMPL)
PTR_LOOP(PTR_LOGFN_IMPL)

CMAP_SSET_LOOP(CMAP_SSET_IMPL)
