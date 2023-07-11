
#include "cmap-cmp-double-handler.h"

#include "cmap-double.h"

/*******************************************************************************
*******************************************************************************/

static int cmp(CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  double double_l = CMAP_CALL((CMAP_DOUBLE *)map_l, get),
    double_r = CMAP_CALL((CMAP_DOUBLE *)map_r, get);
  if(double_l < double_r) return -1;
  else if(double_l > double_r) return 1;
  return 0;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMP_HANDLER cmap_cmp_double_handler = {cmp};
