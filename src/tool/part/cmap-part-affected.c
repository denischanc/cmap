
#include "cmap-part-affected.h"

#include <stdlib.h>
#include "cmap-part-keys.h"

/*******************************************************************************
*******************************************************************************/

static void add(const char * map, const char * name, CMAP_PART_CTX * ctx)
{
  cmap_part_keys_public.add(cmap_part_ctx_public.affecteds(ctx), map, name);
}

/*******************************************************************************
*******************************************************************************/

static char contains_n_add(const char * map, const char * name)
{
  if(!cmap_part_keys_public.contains(*cmap_part_ctx_public.affecteds(NULL),
    map, name))
  {
    add(map, name, NULL);
    return (1 == 0);
  }
  else return (1 == 1);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_AFFECTED_PUBLIC cmap_part_affected_public = {add,
  contains_n_add};
