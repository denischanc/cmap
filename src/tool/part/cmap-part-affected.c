
#include "cmap-part-affected.h"

#include <stdlib.h>
#include "cmap-strings.h"

/*******************************************************************************
*******************************************************************************/

static char add(const char * map, CMAP_PART_CTX * ctx)
{
  char ret = cmap_strings_public.add(cmap_part_ctx_public.affecteds(ctx), map);

  while((ctx = cmap_part_ctx_public.block_next(ctx)) != NULL)
    cmap_strings_public.add(cmap_part_ctx_public.affecteds(ctx), map);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_AFFECTED_PUBLIC cmap_part_affected_public = {add};
