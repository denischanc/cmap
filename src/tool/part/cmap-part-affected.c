
#include "cmap-part-affected.h"

#include <stdlib.h>
#include "cmap-strings.h"

/*******************************************************************************
*******************************************************************************/

static char add(const char * map, CMAP_PART_CTX * ctx)
{
  CMAP_PART_CTX * prev = ctx;
  while((prev = cmap_part_ctx_public.block_prev(prev)) != NULL)
  {
    int off =
      cmap_strings_public.contains(*cmap_part_ctx_public.affecteds(prev), map);
    if(off >= 0) return (1 == 0);
  }

  return cmap_strings_public.add(cmap_part_ctx_public.affecteds(ctx), map);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_AFFECTED_PUBLIC cmap_part_affected_public = {add};
