
#include "cmap-sset.h"

#include <stdio.h>

/*******************************************************************************
*******************************************************************************/

#define PTR_LOOP(macro) \
  macro(LC, lc, CMAP_LIFECYCLE *)

/*******************************************************************************
*******************************************************************************/

#define PTR_EVALFN_IMPL(NAME, name, type) \
static inline int64_t name##_eval(type v_l, type v_r) \
{ \
  return (void *)v_l - (void *)v_r; \
}

/*******************************************************************************
*******************************************************************************/

#define PTR_LOGFN_IMPL(NAME, name, type) \
static inline const char * name##_log_v(type v) \
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
