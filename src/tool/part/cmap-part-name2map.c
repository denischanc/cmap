
#include "cmap-part-name2map.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-part-kv.h"
#include "cmap-part-affected.h"
#include "cmap-part-params.h"
#include "cmap-part-ctx.h"
#include "cmap-parser-util.h"
#include "cmap-part-this-args.h"

/*******************************************************************************
*******************************************************************************/

static void put(const char * map, const char * name, const char * map_name)
{
  cmap_part_kv_public.put(cmap_part_ctx_public.name2map(NULL), map, name,
    map_name);
  cmap_part_affected_public.add(map, name, NULL);

  CMAP_PART_CTX * ctx = cmap_part_ctx_public.c_prev(NULL);
  while(ctx != NULL)
  {
    cmap_part_kv_public.delete_key(cmap_part_ctx_public.name2map(ctx), map,
      name);
    ctx = cmap_part_ctx_public.c_prev(ctx);
  }
}

/*******************************************************************************
*******************************************************************************/

static const char * get_map(const char * map, const char * name,
  CMAP_PART_CTX * ctx)
{
  const char * map_name = cmap_part_kv_public.get(
    *cmap_part_ctx_public.name2map(ctx), map, name);
  if(map_name != NULL)
  {
    CMAP_PART_CTX * ctx_bup =
      cmap_part_ctx_public.bup(cmap_part_ctx_public.last_block(ctx));
    free(cmap_parser_util_public.path((map == NULL) ? NULL : strdup(map),
      strdup(name)));
    cmap_part_ctx_public.restore(ctx_bup);

    return map_name;
  }

  if(!cmap_part_ctx_public.is_params(ctx) &&
    !cmap_part_this_args_public.is(map, name)) return NULL;

  CMAP_PART_CTX * ctx_prev = cmap_part_ctx_public.c_prev(ctx);
  if(ctx_prev == NULL) return NULL;
  map_name = get_map(map, name, ctx_prev);
  if(map_name == NULL) return NULL;

  cmap_part_params_public.add(map_name, ctx);
  cmap_part_kv_public.put(cmap_part_ctx_public.name2map(ctx), map, name,
    map_name);
  cmap_part_affected_public.add(map, name, ctx);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_NAME2MAP_RET get(const char * map, const char * name,
  const char * next_name)
{
  CMAP_PART_NAME2MAP_RET ret;

  const char * map_name = cmap_part_kv_public.get(
    *cmap_part_ctx_public.name2map(NULL), map, name);
  if(map_name != NULL)
  {
    ret.map = map_name;
    ret.affected = cmap_part_affected_public.contains_n_add(map, name);
  }
  else
  {
    map_name = get_map(map, name, cmap_part_ctx_public.c());
    if(map_name != NULL)
    {
      ret.map = map_name;
      ret.affected = (1 == 1);
    }
    else
    {
      cmap_part_kv_public.put(cmap_part_ctx_public.name2map(NULL), map, name,
        next_name);
      cmap_part_affected_public.add(map, name, NULL);

      ret.map = next_name;
    }
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_NAME2MAP_PUBLIC cmap_part_name2map_public = {put, get};
