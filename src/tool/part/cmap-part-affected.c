
#include "cmap-part-affected.h"

#include <stdlib.h>
#include "cmap-strings.h"

/*******************************************************************************
*******************************************************************************/

char cmap_part_affected_add(const char * map, CMAP_PART_CTX * ctx)
{
  CMAP_PART_CTX * prev = ctx;
  while((prev = cmap_part_ctx_block_prev(prev)) != NULL)
  {
    int off = cmap_strings_contains(*cmap_part_ctx_affecteds(prev), map);
    if(off >= 0) return (1 == 0);
  }

  return cmap_strings_add(cmap_part_ctx_affecteds(ctx), map);
}
