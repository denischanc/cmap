
#include "cmap-sset.h"

#include <stdio.h>

/*******************************************************************************
*******************************************************************************/

#define PTR_LOOP(macro) \
  macro(LC, lc, CMAP_LIFECYCLE *)

/*******************************************************************************
*******************************************************************************/

#define PTR_EVALFN_IMPL(NAME, name, type) \
static int name##_eval(type v_l, type v_r) \
{ \
  if(v_l > v_r) return 1; \
  else if(v_l < v_r) return -1; \
  else return 0; \
}

/*******************************************************************************
*******************************************************************************/

#define PTR_LOGFN_IMPL(NAME, name, type) \
static const char * name##_log_v(type v) \
{ \
  static char buffer[20]; \
  snprintf(buffer, sizeof(buffer), "%p", v); \
  return buffer; \
}

/*******************************************************************************
*******************************************************************************/

PTR_LOOP(PTR_EVALFN_IMPL)
PTR_LOOP(PTR_LOGFN_IMPL)

CMAP_SSET_LOOP(CMAP_SSET_IMPL)
