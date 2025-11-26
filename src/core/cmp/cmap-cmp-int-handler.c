
#include "cmap-cmp-int-handler.h"

#include "cmap-int.h"

/*******************************************************************************
*******************************************************************************/

static int64_t cmp(CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  int64_t int_l = CMAP_CALL((CMAP_INT *)map_l, get),
    int_r = CMAP_CALL((CMAP_INT *)map_r, get);
  return int_l - int_r;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMP_PUBLIC cmap_cmp_int_handler_public = {cmp};
