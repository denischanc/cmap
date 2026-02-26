
#include "cmap-cmp-handler.h"

#include "cmap-double.h"

/*******************************************************************************
*******************************************************************************/

static char cmp(int64_t * ret, CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  if((map_l == NULL) || (map_r == NULL) ||
    (CMAP_NATURE(map_l) != CMAP_DOUBLE_NATURE) ||
    (CMAP_NATURE(map_r) != CMAP_DOUBLE_NATURE)) return (1 == 0);

  double double_l = CMAP_CALL((CMAP_DOUBLE *)map_l, get),
    double_r = CMAP_CALL((CMAP_DOUBLE *)map_r, get);
  if(double_l == double_r) *ret = 0;
  else *ret = (double_l > double_r) ? 1 : -1;
  return (1 == 1);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMP_HANDLER cmap_cmp_handler_double = {cmp};
