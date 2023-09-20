
#include "cmap-cmp-int-handler.h"

#include "cmap-int.h"

/*******************************************************************************
*******************************************************************************/

static int cmp(CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  int64_t int_l = CMAP_CALL((CMAP_INT *)map_l, get),
    int_r = CMAP_CALL((CMAP_INT *)map_r, get);
  if(int_l < int_r) return -1;
  else if(int_l > int_r) return 1;
  return 0;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMP_HANDLER_PUBLIC cmap_cmp_int_handler_public = {cmp};
