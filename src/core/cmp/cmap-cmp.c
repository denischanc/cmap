
#include "cmap-cmp.h"

#include "cmap-cmp-handler.h"
#include "cmap-int.h"
#include "cmap.h"

/*******************************************************************************
*******************************************************************************/

#define IMPL(type) \
  if(cmap_cmp_handler_##type.cmp(&ret, map_l, map_r)) return ret;

int64_t cmap_cmp_run(CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  int64_t ret = (void *)map_l - (void *)map_r;
  if(ret == 0) return 0;

  CMAP_CMP_HANDLER_LOOP(IMPL)
  return ret;
}

/*******************************************************************************
*******************************************************************************/

char cmap_cmp_is_true(CMAP_MAP * map)
{
  if(map == NULL) return (1 == 0);

  if(CMAP_NATURE(map) == CMAP_INT_NATURE)
    return (CMAP_CALL((CMAP_INT *)map, get) != 0);
  return (1 == 1);
}
