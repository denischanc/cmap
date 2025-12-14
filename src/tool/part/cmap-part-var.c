
#include "cmap-part-var.h"

#include <stdlib.h>
#include "cmap-strings.h"
#include "cmap-parser-this-args.h"

/*******************************************************************************
*******************************************************************************/

static char is_fn_arg_name(const char * name, CMAP_PART_CTX * ctx)
{
  return ((cmap_strings_public.contains(
    cmap_part_ctx_public.prev_block_fn_arg_names(ctx), name) >= 0) ||
    cmap_parser_this_args_public.is(NULL, name));
}

static char is_loc(const char * name, CMAP_PART_CTX * ctx)
{
  return (cmap_strings_public.contains(
    *cmap_part_ctx_public.vars_loc(ctx), name) >= 0);
}

static char is_def(const char * name, CMAP_PART_CTX * ctx)
{
  return (cmap_strings_public.contains(
    *cmap_part_ctx_public.vars_def(ctx), name) >= 0);
}

static char is_proc_ctx_def(const char * name, CMAP_PART_CTX * ctx)
{
  return (is_fn_arg_name(name, ctx) || is_loc(name, ctx) || is_def(name, ctx));
}

/*******************************************************************************
*******************************************************************************/

static void put_loc(const char * name, const char * next_name)
{
  cmap_part_name2map_public.put(NULL, name, next_name);

  cmap_strings_public.add(cmap_part_ctx_public.vars_loc(NULL), name);
}

/*******************************************************************************
*******************************************************************************/

static char put_no_loc_is_def(const char * name)
{
  if(is_fn_arg_name(name, NULL))
  {
    cmap_strings_public.add(cmap_part_ctx_public.vars_loc(NULL), name);
    return (1 == 1);
  }
  if(is_loc(name, NULL) || is_def(name, NULL)) return (1 == 1);

  CMAP_PART_CTX * ctx = NULL;
  while((ctx = cmap_part_ctx_public.cmap_prev(ctx)) != NULL)
    if(is_proc_ctx_def(name, ctx)) return (1 == 1);

  return (1 == 0);
}

static char put_no_loc(const char * map, const char * name,
  const char * next_name)
{
  cmap_part_name2map_public.put(map, name, next_name);

  return (map == NULL) ? put_no_loc_is_def(name) : (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

static char get_is_def(const char * name, CMAP_PART_CTX * ctx)
{
  if(is_proc_ctx_def(name, ctx)) return (1 == 1);

  CMAP_PART_CTX * ctx_prev = cmap_part_ctx_public.cmap_prev(ctx);
  if(ctx_prev == NULL) return (1 == 0);

  if(!get_is_def(name, ctx_prev)) return (1 == 0);

  cmap_strings_public.add(cmap_part_ctx_public.vars_def(ctx), name);
  return (1 == 1);
}

static CMAP_PART_VAR_RET get(const char * map, const char * name,
  char * next_name)
{
  CMAP_PART_VAR_RET ret;
  ret.ret = cmap_part_name2map_public.get(map, name, next_name);
  ret.is_def = (map == NULL) ? get_is_def(name, NULL) : (1 == 0);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_VAR_PUBLIC cmap_part_var_public =
  {is_loc, is_proc_ctx_def, put_loc, put_no_loc, get};
