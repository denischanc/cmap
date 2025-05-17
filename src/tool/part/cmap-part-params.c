
#include "cmap-part-params.h"

#include "cmap-strings.h"

/*******************************************************************************
*******************************************************************************/

static void add(const char * map, CMAP_PART_CTX * ctx)
{
  cmap_strings_public.add(cmap_part_ctx_public.params(ctx), map);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_PARAMS_PUBLIC cmap_part_params_public = {add};
