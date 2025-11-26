
#include "cmap-cmp-double-handler.h"

#include "cmap-double.h"

/*******************************************************************************
*******************************************************************************/

static int64_t cmp(CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  double double_l = CMAP_CALL((CMAP_DOUBLE *)map_l, get),
    double_r = CMAP_CALL((CMAP_DOUBLE *)map_r, get);
  if(double_l == double_r) return 0;
  else return (double_l > double_r) ? 1 : -1;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMP_PUBLIC cmap_cmp_double_handler_public = {cmp};
