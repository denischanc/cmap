
#include "cmap-cmp.h"

#include "cmap-cmp-handler.h"
#include "cmap-map.h"

/*******************************************************************************
*******************************************************************************/

#define IMPL(type) \
  if(cmap_cmp_handler_##type##_public.cmp(&ret, map_l, map_r)) return ret;

static int64_t cmp(CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  int64_t ret = map_l - map_r;
  if(ret == 0) return 0;

  CMAP_CMP_HANDLER_LOOP(IMPL)
  return ret;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMP_PUBLIC cmap_cmp_public = {cmp};
