
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
  cmap_part_kv_put(cmap_part_ctx_name2map(ctx), map, name, map_name);
  cmap_part_affected_add(map_name, ctx);
}

void cmap_part_name2map_put(const char * map, const char * name,
  const char * next_name)
{
  put_n_affected(map, name, next_name, NULL);

  CMAP_PART_CTX * ctx = NULL;
  while((ctx = cmap_part_ctx_c_prev(ctx)) != NULL)
    cmap_part_kv_delete_key(cmap_part_ctx_name2map(ctx), map, name);
}

/*******************************************************************************
*******************************************************************************/

static char is_this_args(const char * map, const char * name,
  CMAP_PART_CTX * ctx)
{
  return cmap_parser_this_args_is(map, name) &&
    !cmap_part_var_is_loc(name, ctx);
}

/*******************************************************************************
*******************************************************************************/

static char * is_fn_arg_name(const char * name, CMAP_PART_CTX * ctx_c)
{
  if(cmap_part_var_is_loc(name, ctx_c)) return NULL;
  if(is_this_args(NULL, name, ctx_c))
    return strdup(cmap_parser_this_args_map(name));

  int off = cmap_strings_contains(
    cmap_part_ctx_prev_block_fn_arg_names(ctx_c), name);
  if(off < 0) return NULL;

  CMAP_PART_CTX * ctx_split = cmap_part_ctx_split(ctx_c);
  char * map_name = cmap_parser_var_set_fn_arg_name(name, off);
  cmap_part_ctx_join(ctx_split);

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

static char is_new(const char * map, const char * name, CMAP_PART_CTX * ctx_c)
{
  return !cmap_part_ctx_is_feature_fwd_name(ctx_c) &&
    !is_this_args(map, name, ctx_c);
}

static void not_new_path(const char * map, const char * name,
  CMAP_PART_CTX * ctx_c)
{
  CMAP_PART_CTX * ctx_split =
    cmap_part_ctx_split(cmap_part_ctx_last_block(ctx_c));
  free(cmap_parser_var_path((map == NULL) ? NULL : strdup(map), strdup(name)));
  cmap_part_ctx_join(ctx_split);
}

static void mng_not_new(const char * map, const char * name,
  const char * map_name, CMAP_PART_CTX * ctx_c)
{
  if(cmap_part_ctx_is_feature_ctx_fn_c(ctx_c))
  {
    cmap_strings_add(cmap_part_ctx_params(ctx_c), map_name);
    put_n_affected(map, name, map_name, ctx_c);
  }
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_NAME2MAP_RET get_prev(const char * map, const char * name,
  char new, CMAP_PART_CTX * ctx_c)
{
  CMAP_PART_NAME2MAP_RET ret;

  const char * map_name_ok = cmap_part_kv_get(
    *cmap_part_ctx_name2map(ctx_c), map, name);
  if(map_name_ok != NULL)
  {
    if(!new) not_new_path(map, name, ctx_c);

    return create_ret(strdup(map_name_ok), new, (1 == 1));
  }

  CMAP_PART_CTX * ctx_c_prev = cmap_part_ctx_c_prev(ctx_c);
  if(ctx_c_prev == NULL) return create_ret(
    (map == NULL) ? is_fn_arg_name(name, ctx_c) : NULL, new, (1 == 1));
  else
  {
    ret = get_prev(map, name, new || is_new(map, name, ctx_c), ctx_c_prev);
    if((ret.map != NULL) && !ret.new) mng_not_new(map, name, ret.map, ctx_c);
    return ret;
  }
}

/*******************************************************************************
*******************************************************************************/

CMAP_PART_NAME2MAP_RET cmap_part_name2map_get(const char * map,
  const char * name, char * next_name)
{
  const char * map_name_ok = cmap_part_kv_get(
    *cmap_part_ctx_name2map(NULL), map, name);
  if(map_name_ok != NULL) return create_ret(strdup(map_name_ok), (1 == 0),
    !cmap_part_affected_add(map_name_ok, NULL));
  else
  {
    CMAP_PART_NAME2MAP_RET ret =
      get_prev(map, name, (1 == 0), cmap_part_ctx_c());

    if(ret.map == NULL) put_n_affected(map, name, next_name, NULL);
    else if(ret.new) put_n_affected(map, name, ret.map, NULL);

    return ret;
  }
}

/*******************************************************************************
*******************************************************************************/

static char clean_after_proc_apply(const char * map, const char * name,
  const char * map_name, void * data)
{
  CMAP_PART_CTX * ctx = data;
  return ((map != NULL) || !cmap_part_var_is_proc_ctx_def(name, ctx));
}

void cmap_part_name2map_clean_after_proc()
{
  CMAP_PART_CTX * ctx_c = cmap_part_ctx_c();
  while(ctx_c != NULL)
  {
    cmap_part_kv_apply(cmap_part_ctx_name2map(ctx_c), clean_after_proc_apply,
      ctx_c);

    ctx_c = cmap_part_ctx_c_prev(ctx_c);
  }
}
