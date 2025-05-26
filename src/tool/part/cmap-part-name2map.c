
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

static char is_fn_arg_name(const char * name, CMAP_PART_CTX * ctx_c)
{
  CMAP_PART_CTX * ctx_c_prev = cmap_part_ctx_public.c_prev(ctx_c);
  if(ctx_c_prev != NULL)
  {
    if(cmap_part_kv_public.get(*cmap_part_ctx_public.name2map(ctx_c_prev),
      NULL, name) != NULL) return (1 == 0);
    return is_fn_arg_name(name, ctx_c_prev);
  }

  if(cmap_part_this_args_public.is(NULL, name)) return (1 == 1);

  int off = cmap_strings_public.contains(
    *cmap_part_ctx_public.block_fn_arg_names(ctx_c), name);
  if(off == -1) return (1 == 0);

  CMAP_PART_CTX * ctx_bup = cmap_part_ctx_public.bup(ctx_c);
  cmap_parser_util_public.set_fn_arg_name(strdup(name), off);
  cmap_part_ctx_public.restore(ctx_bup);

  return (1 == 1);
}

/*******************************************************************************
*******************************************************************************/

static const char * get_map_by_params(const char * map, const char * name,
  CMAP_PART_CTX * ctx_c)
{
  const char * map_name = cmap_part_kv_public.get(
    *cmap_part_ctx_public.name2map(ctx_c), map, name);
  if(map_name != NULL)
  {
    CMAP_PART_CTX * ctx_bup =
      cmap_part_ctx_public.bup(cmap_part_ctx_public.last_block(ctx_c));
    free(cmap_parser_util_public.path((map == NULL) ? NULL : strdup(map),
      strdup(name)));
    cmap_part_ctx_public.restore(ctx_bup);

    return map_name;
  }

  CMAP_PART_CTX * ctx_c_prev = cmap_part_ctx_public.c_prev(ctx_c);
  if(ctx_c_prev == NULL)
  {
    if((map == NULL) && is_fn_arg_name(name, ctx_c)) map_name = name;
    else return NULL;
  }
  else
  {
    if(!cmap_part_ctx_public.is_params(ctx_c) &&
      !cmap_part_this_args_public.is(map, name))
    {
      if(map == NULL) is_fn_arg_name(name, ctx_c);
      return NULL;
    }

    map_name = get_map_by_params(map, name, ctx_c_prev);
    if(map_name == NULL) return NULL;

    cmap_part_params_public.add(map_name, ctx_c);
    cmap_part_kv_public.put(cmap_part_ctx_public.name2map(ctx_c), map, name,
      map_name);
  }

  cmap_part_affected_public.add(map, name, ctx_c);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_NAME2MAP_RET get(const char * map, const char * name,
  char * next_name)
{
  CMAP_PART_NAME2MAP_RET ret;

  const char * map_name = cmap_part_kv_public.get(
    *cmap_part_ctx_public.name2map(NULL), map, name);
  if(map_name != NULL)
  {
    ret.map = strdup(map_name);
    ret.affected = cmap_part_affected_public.contains_n_add(map, name);
  }
  else
  {
    map_name = get_map_by_params(map, name, cmap_part_ctx_public.c());
    if(map_name != NULL)
    {
      ret.map = strdup(map_name);
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
