
#include "cmap-part-name2map.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-part-kv.h"
#include "cmap-part-affected.h"
#include "cmap-part-ctx.h"
#include "cmap-part-var.h"
#include "cmap-parser-var.h"
#include "cmap-parser-this-args.h"

/*******************************************************************************
*******************************************************************************/

static void put_n_affected(const char * map, const char * name,
  const char * map_name, CMAP_PART_CTX * ctx)
{
  cmap_part_kv_public.put(cmap_part_ctx_public.name2map(ctx), map, name,
    map_name);
  cmap_part_affected_public.add(map_name, ctx);
}

static CMAP_PART_NAME2MAP_RET put(const char * map, const char * name,
  const char * next_name)
{
  CMAP_PART_NAME2MAP_RET ret;

  const char * map_name =
    cmap_part_kv_public.get(*cmap_part_ctx_public.name2map(NULL), map, name);
  if(map_name == NULL)
  {
    ret.map = NULL;
    put_n_affected(map, name, next_name, NULL);
  }
  else
  {
    ret.map = strdup(map_name);
    cmap_part_affected_public.add(map_name, NULL);
  }

  CMAP_PART_CTX * ctx = NULL;
  while((ctx = cmap_part_ctx_public.c_prev(ctx)) != NULL)
    cmap_part_kv_public.delete_key(cmap_part_ctx_public.name2map(ctx), map,
      name);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * is_fn_arg_name(const char * name, CMAP_PART_CTX * ctx_c)
{
  if(cmap_parser_this_args_public.is(NULL, name)) return strdup(name);

  int off = cmap_strings_public.contains(
    cmap_part_ctx_public.prev_block_fn_arg_names(ctx_c), name);
  if(off < 0) return NULL;

  CMAP_PART_CTX * ctx_split = cmap_part_ctx_public.split(ctx_c);
  char * map_name = cmap_parser_var_public.set_fn_arg_name(name, off);
  cmap_part_ctx_public.join(ctx_split);

  cmap_strings_public.set(
    cmap_part_ctx_public.prev_block_fn_arg_names(ctx_c), off, "");

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static inline CMAP_PART_NAME2MAP_RET create_ret(char * map, char new,
  char affected)
{
  CMAP_PART_NAME2MAP_RET ret;
  ret.map = map;
  ret.new = new;
  ret.affected = affected;
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_NAME2MAP_RET get_map_by_params(const char * map,
  const char * name, char new, CMAP_PART_CTX * ctx_c)
{
  CMAP_PART_NAME2MAP_RET ret;

  const char * map_name_ok = cmap_part_kv_public.get(
    *cmap_part_ctx_public.name2map(ctx_c), map, name);
  if(map_name_ok != NULL)
  {
    if(!new)
    {
      CMAP_PART_CTX * ctx_split =
        cmap_part_ctx_public.split(cmap_part_ctx_public.last_block(ctx_c));
      free(cmap_parser_var_public.path((map == NULL) ? NULL : strdup(map),
        strdup(name)));
      cmap_part_ctx_public.join(ctx_split);
    }

    return create_ret(strdup(map_name_ok), new, (1 == 1));
  }

  CMAP_PART_CTX * ctx_c_prev = cmap_part_ctx_public.c_prev(ctx_c);
  if(ctx_c_prev == NULL) return create_ret(
    (map == NULL) ? is_fn_arg_name(name, ctx_c) : NULL, new, (1 == 1));
  else
  {
    ret = get_map_by_params(map, name, new ||
      (!cmap_part_ctx_public.is_feature_params(ctx_c) &&
      !cmap_parser_this_args_public.is(map, name)), ctx_c_prev);
    if((ret.map != NULL) && !ret.new)
    {
      cmap_strings_public.add(cmap_part_ctx_public.params(ctx_c), ret.map);
      put_n_affected(map, name, ret.map, ctx_c);
    }
    return ret;
  }
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_NAME2MAP_RET get(const char * map, const char * name,
  char * next_name)
{
  CMAP_PART_NAME2MAP_RET ret;

  const char * map_name_ok = cmap_part_kv_public.get(
    *cmap_part_ctx_public.name2map(NULL), map, name);
  if(map_name_ok != NULL) ret = create_ret(strdup(map_name_ok), (1 == 0),
    !cmap_part_affected_public.add(map_name_ok, NULL));
  else
  {
    ret = get_map_by_params(map, name, (1 == 0), cmap_part_ctx_public.c());

    if(ret.map == NULL) put_n_affected(map, name, next_name, NULL);
    else if(ret.new) put_n_affected(map, name, ret.map, NULL);
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char proc_clean_apply(const char * map, const char * name,
  const char * map_name, void * data)
{
  CMAP_PART_CTX * ctx = data;
  return ((map != NULL) || !cmap_part_var_public.proc_is_local(name, ctx));
}

static void proc_clean()
{
  CMAP_PART_CTX * ctx_c = cmap_part_ctx_public.c();
  while(ctx_c != NULL)
  {
    cmap_part_kv_public.apply(cmap_part_ctx_public.name2map(ctx_c),
      proc_clean_apply, ctx_c);

    ctx_c = cmap_part_ctx_public.c_prev(ctx_c);
  }
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_NAME2MAP_PUBLIC cmap_part_name2map_public =
  {put, get, proc_clean};
