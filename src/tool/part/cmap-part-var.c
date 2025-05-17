
#include "cmap-part-var.h"

#include <stdlib.h>
#include "cmap-strings.h"
#include "cmap-part-ctx.h"

/*******************************************************************************
*******************************************************************************/

static char is_loc_cur(const char * name, CMAP_PART_CTX * ctx)
{
  return cmap_strings_public.contains(
    *cmap_part_ctx_public.vars_loc(ctx), name);
}

static char is_def_cur(const char * name, CMAP_PART_CTX * ctx)
{
  return cmap_strings_public.contains(
    *cmap_part_ctx_public.vars_def(ctx), name);
}

static char is_loc_or_def_cur(const char * name, CMAP_PART_CTX * ctx)
{
  return (is_loc_cur(name, ctx) || is_def_cur(name, ctx));
}

/*******************************************************************************
*******************************************************************************/

static char is_loc_prev(const char * name)
{
  CMAP_PART_CTX * ctx = cmap_part_ctx_public.cmap_prev(NULL);
  while(ctx != NULL)
  {
    if(is_loc_cur(name, ctx)) return (1 == 1);

    ctx = cmap_part_ctx_public.cmap_prev(ctx);
  }

  return (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

static void put_loc(const char * name, const char * map)
{
  cmap_part_name2map_public.put(NULL, name, map);

  cmap_strings_public.add(cmap_part_ctx_public.vars_loc(NULL), name);
}

/*******************************************************************************
*******************************************************************************/

static char put_no_loc(const char * map, const char * name,
  const char * map_name)
{
  cmap_part_name2map_public.put(map, name, map_name);

  if(map != NULL) return (1 == 0);

  if(is_loc_or_def_cur(name, NULL)) return (1 == 1);

  if(is_loc_prev(name))
  {
    cmap_strings_public.add(cmap_part_ctx_public.vars_loc(NULL), name);
    return (1 == 1);
  }

  return (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

static char is_def(const char * name, CMAP_PART_CTX * ctx)
{
  if(is_loc_or_def_cur(name, ctx)) return (1 == 1);

  CMAP_PART_CTX * ctx_prev = cmap_part_ctx_public.cmap_prev(ctx);
  if(ctx_prev == NULL) return (1 == 0);

  if(!is_def(name, ctx_prev)) return (1 == 0);

  cmap_strings_public.add(cmap_part_ctx_public.vars_def(ctx), name);
  return (1 == 1);
}

static CMAP_PART_VAR_RET get(const char * map, const char * name,
  const char * next_name)
{
  CMAP_PART_VAR_RET ret;
  ret.ret = cmap_part_name2map_public.get(map, name, next_name);
  ret.is_def = (map == NULL) ? is_def(name, NULL) : (1 == 0);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_VAR_PUBLIC cmap_part_var_public = {put_loc, put_no_loc, get};
