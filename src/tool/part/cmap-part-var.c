
#include "cmap-part-var.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-strings.h"

/*******************************************************************************
*******************************************************************************/

static char is_fn_arg_name(const char * name, CMAP_PART_CTX * ctx)
{
  return (cmap_strings_public.contains(
    cmap_part_ctx_public.prev_block_fn_arg_names(ctx), name) >= 0);
}

static char is_loc_ctx(const char * name, CMAP_PART_CTX * ctx)
{
  return ((cmap_strings_public.contains(
    *cmap_part_ctx_public.vars_loc(ctx), name) >= 0) ||
    is_fn_arg_name(name, ctx));
}

static char is_def_ctx(const char * name, CMAP_PART_CTX * ctx)
{
  return (cmap_strings_public.contains(
    *cmap_part_ctx_public.vars_def(ctx), name) >= 0);
}

static char is_loc_or_def_ctx(const char * name, CMAP_PART_CTX * ctx)
{
  return (is_loc_ctx(name, ctx) || is_def_ctx(name, ctx));
}

/*******************************************************************************
*******************************************************************************/

static char proc_is_local(const char * name, CMAP_PART_CTX * ctx)
{
  int off_loc =
    cmap_strings_public.contains(*cmap_part_ctx_public.vars_loc(ctx), name);
  int off_def =
    cmap_strings_public.contains(*cmap_part_ctx_public.vars_def(ctx), name);
  return ((off_loc >= 0) || (off_def >= 0));
}

/*******************************************************************************
*******************************************************************************/

static char is_loc_prev(const char * name)
{
  CMAP_PART_CTX * ctx = cmap_part_ctx_public.cmap_prev(NULL);
  while(ctx != NULL)
  {
    if(is_loc_ctx(name, ctx)) return (1 == 1);

    ctx = cmap_part_ctx_public.cmap_prev(ctx);
  }

  return (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_VAR_RET put_loc(const char * name, const char * next_name)
{
  CMAP_PART_VAR_RET ret;

  ret.ret = cmap_part_name2map_public.put(NULL, name, next_name);

  cmap_strings_public.add(cmap_part_ctx_public.vars_loc(NULL), name);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_VAR_RET put_no_loc(const char * map, const char * name,
  const char * next_name)
{
  CMAP_PART_VAR_RET ret;

  ret.ret = cmap_part_name2map_public.put(map, name, next_name);

  if(map == NULL)
    ret.is_def = (is_loc_or_def_ctx(name, NULL) || is_loc_prev(name));

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char is_def(const char * name, CMAP_PART_CTX * ctx)
{
  if(is_loc_or_def_ctx(name, ctx)) return (1 == 1);

  CMAP_PART_CTX * ctx_prev = cmap_part_ctx_public.cmap_prev(ctx);
  if(ctx_prev == NULL) return (1 == 0);

  if(!is_def(name, ctx_prev)) return (1 == 0);

  cmap_strings_public.add(cmap_part_ctx_public.vars_def(ctx), name);
  return (1 == 1);
}

static CMAP_PART_VAR_RET get(const char * map, const char * name,
  char * next_name)
{
  CMAP_PART_VAR_RET ret;
  ret.is_def = (map == NULL) ? is_def(name, NULL) : (1 == 0);
  ret.ret = cmap_part_name2map_public.get(map, name, next_name);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_VAR_PUBLIC cmap_part_var_public =
  {proc_is_local, put_loc, put_no_loc, get};
