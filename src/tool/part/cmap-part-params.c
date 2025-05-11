
#include "cmap-part-params.h"

#include <stdlib.h>
#include "cmap-part-kv.h"
#include "cmap-part-var.h"
#include "cmap-strings.h"

/*******************************************************************************
*******************************************************************************/

static const char * get(const char * name, CMAP_PART_CTX * ctx)
{
  CMAP_PART_CTX_C * c = ctx -> block.c;
  const char * map = cmap_part_kv_public.get(c -> prev -> name2map, name);

  if(map == NULL) return NULL;

  cmap_part_var_public.put(name, map, ctx);
  cmap_strings_public.add(&c -> params, name);

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * this(CMAP_PART_CTX * ctx)
{
  return ctx -> block.c -> params;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_PARAMS_PUBLIC cmap_part_params_public = {get, this};
