
#include "cmap-cmp-string-handler.h"

#include <string.h>
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static int cmp(CMAP_MAP * map_l, CMAP_MAP * map_r)
{
  const char * string_l = CMAP_CALL((CMAP_STRING *)map_l, val),
    * string_r = CMAP_CALL((CMAP_STRING *)map_r, val);
  return strcmp(string_l, string_r);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CMP_HANDLER_PUBLIC cmap_cmp_string_handler_public = {cmp};
