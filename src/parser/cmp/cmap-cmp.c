
#include "cmap-cmp.h"

#include "cmap-proc-ctx.h"
#include "cmap-int.h"
#include "cmap-cmp-int-handler.h"
#include "cmap-double.h"
#include "cmap-cmp-double-handler.h"
#include "cmap-string.h"
#include "cmap-cmp-string-handler.h"

/*******************************************************************************
*******************************************************************************/

static const CMAP_CMP_HANDLER * handler(const char * nature_l,
  const char * nature_r)
{
  if(nature_l == nature_r)
  {
    if(nature_l == CMAP_INT_NATURE) return &cmap_cmp_int_handler;
    else if(nature_l == CMAP_DOUBLE_NATURE) return &cmap_cmp_double_handler;
    else if(nature_l == CMAP_STRING_NATURE) return &cmap_cmp_string_handler;
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_INT * take(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_POOL_INT * pool = CMAP_CALL(proc_ctx, pool_int);
  return CMAP_CALL_ARGS(pool, take, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

#define CMP_IMPL(name, cmp_) \
static CMAP_MAP * name(CMAP_MAP * map_l, CMAP_MAP * map_r, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  int cmp; \
  const CMAP_CMP_HANDLER * handler_ = handler(CMAP_NATURE(map_l), \
    CMAP_NATURE(map_r)); \
  if(handler_ != NULL) cmp = handler_ -> cmp(map_l, map_r); \
  else cmp = map_l - map_r; \
 \
  CMAP_INT * i = take(proc_ctx); \
  CMAP_CALL_ARGS(i, set, cmp cmp_ 0); \
  return (CMAP_MAP *)i; \
}

CMAP_CMP_LOOP(CMP_IMPL)

/*******************************************************************************
*******************************************************************************/

#define CMP_SET(name, cmp) name,

const CMAP_CMP_PUBLIC cmap_cmp_public =
{
  CMAP_CMP_LOOP(CMP_SET)
};
