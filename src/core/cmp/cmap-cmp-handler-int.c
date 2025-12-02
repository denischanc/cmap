
#include "cmap-cmp-handler.h"

#include "cmap-int.h"

/*******************************************************************************
*******************************************************************************/

static char cmp(int64_t * ret, CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  if((map_l == NULL) || (map_r == NULL) ||
    (CMAP_NATURE(map_l) != CMAP_INT_NATURE) ||
    (CMAP_NATURE(map_r) != CMAP_INT_NATURE)) return (1 == 0);

  int64_t int_l = CMAP_CALL((CMAP_INT *)map_l, get),
    int_r = CMAP_CALL((CMAP_INT *)map_r, get);
  *ret = int_l - int_r;
  return (1 == 1);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMP_HANDLER cmap_cmp_handler_int_public = {cmp};
