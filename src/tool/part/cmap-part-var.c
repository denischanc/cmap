
#include "cmap-part-var.h"

#include <stdlib.h>
#include "cmap-strings.h"
#include "cmap-part-kv.h"

/*******************************************************************************
*******************************************************************************/

static char is_loc_cur(const char * name, CMAP_PART_CTX_CMAP * cmap)
{
  return cmap_strings_public.contains(cmap -> vars_loc, name);
}

static char is_def_cur(const char * name, CMAP_PART_CTX_CMAP * cmap)
{
  return cmap_strings_public.contains(cmap -> vars_def, name);
}

static char is_loc_or_def_cur(const char * name, CMAP_PART_CTX_CMAP * cmap)
{
  return (is_loc_cur(name, cmap) || is_def_cur(name, cmap));
}

/*******************************************************************************
*******************************************************************************/

static char is_loc_prev(const char * name, CMAP_PART_CTX_CMAP * cmap)
{
  cmap = cmap -> prev;
  while(cmap != NULL)
  {
    if(is_loc_cur(name, cmap)) return (1 == 1);

    cmap = cmap -> prev;
  }

  return (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

static void put_loc(const char * name, const char * map, CMAP_PART_CTX * ctx)
{
  cmap_part_name2map_public.put(NULL, name, map, ctx);

  CMAP_PART_CTX_CMAP * cmap = ctx -> block.c -> cmap;
  cmap_strings_public.add(&cmap -> vars_loc, name);
}

/*******************************************************************************
*******************************************************************************/

static char put_no_loc(const char * map, const char * name,
  const char * map_name, CMAP_PART_CTX * ctx)
{
  cmap_part_name2map_public.put(map, name, map_name, ctx);

  if(map != NULL) return (1 == 0);

  CMAP_PART_CTX_CMAP * cmap = ctx -> block.c -> cmap;
  if(is_loc_or_def_cur(name, cmap)) return (1 == 1);

  if(is_loc_prev(name, cmap))
  {
    cmap_strings_public.add(&cmap -> vars_loc, name);
    return (1 == 1);
  }

  return (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

static char is_def(const char * name, CMAP_PART_CTX_CMAP * cmap)
{
  if(is_loc_or_def_cur(name, cmap)) return (1 == 1);

  CMAP_PART_CTX_CMAP * cmap_prev = cmap -> prev;
  if(cmap_prev == NULL) return (1 == 0);

  if(!is_def(name, cmap_prev)) return (1 == 0);

  cmap_strings_public.add(&cmap -> vars_def, name);
  return (1 == 1);
}

static CMAP_PART_VAR_RET get(const char * map, const char * name,
  const char * next_name, CMAP_PART_CTX * ctx)
{
  CMAP_PART_VAR_RET ret;
  ret.ret = cmap_part_name2map_public.get(map, name, next_name, ctx);
  ret.is_def = (map == NULL) ? is_def(name, ctx -> block.c -> cmap) : (1 == 0);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * defs(CMAP_PART_CTX * ctx)
{
  return ctx -> block.c -> cmap -> vars_def;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_VAR_PUBLIC cmap_part_var_public =
{
  put_loc, put_no_loc,
  get,
  defs
};
