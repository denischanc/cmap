
#include "cmap-cmp-handler.h"

#include <string.h>
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static char cmp(int64_t * ret, CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  if((map_l == NULL) || (map_r == NULL) ||
    (CMAP_NATURE(map_l) != CMAP_STRING_NATURE) ||
    (CMAP_NATURE(map_r) != CMAP_STRING_NATURE)) return (1 == 0);

  const char * string_l = CMAP_CALL((CMAP_STRING *)map_l, val),
    * string_r = CMAP_CALL((CMAP_STRING *)map_r, val);
  *ret = strcmp(string_l, string_r);
  return (1 == 1);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMP_HANDLER cmap_cmp_handler_string = {cmp};
