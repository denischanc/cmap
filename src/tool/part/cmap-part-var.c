
#include "cmap-part-var.h"

#include <stdlib.h>
#include "cmap-strings.h"
#include "cmap-part-kv.h"
#include "cmap-part-dirty.h"

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

static char is_def(const char * name, CMAP_PART_CTX_CMAP * cmap)
{
  if(is_loc_or_def_cur(name, cmap)) return (1 == 1);

  CMAP_PART_CTX_CMAP * cmap_prev = cmap -> prev;
  if(cmap_prev == NULL) return (1 == 0);

  if(!is_def(name, cmap_prev)) return (1 == 0);

  cmap_strings_public.add(&cmap -> vars_def, name);
  return (1 == 1);
}

/*******************************************************************************
*******************************************************************************/

static void put(const char * map_, const char * name, const char * map_name,
  CMAP_PART_CTX * ctx)
{
  CMAP_PART_CTX_BLOCK * block = &ctx -> block;

  cmap_part_kv_public.put(&block -> c -> name2map, map_, name, map_name);

  cmap_part_dirty_public.name(map_, name, ctx);
}

/*******************************************************************************
*******************************************************************************/

static void put_upd_prev(const char * map_, const char * name,
  const char * map_name, CMAP_PART_CTX * ctx)
{
  put(map_, name, map_name, ctx);

  CMAP_PART_CTX_C * c = ctx -> block.c -> prev;
  while(c != NULL)
  {
    cmap_part_kv_public.delete_key(&c -> name2map, map_, name);

    c = c -> prev;
  }
}

/*******************************************************************************
*******************************************************************************/

static void put_loc_only(const char * name, CMAP_PART_CTX * ctx)
{
  CMAP_PART_CTX_CMAP * cmap = ctx -> block.c -> cmap;
  cmap_strings_public.add(&cmap -> vars_loc, name);
}

/*******************************************************************************
*******************************************************************************/

static void put_loc(const char * name, const char * map, CMAP_PART_CTX * ctx)
{
  put_upd_prev(NULL, name, map, ctx);
  put_loc_only(name, ctx);
}

/*******************************************************************************
*******************************************************************************/

static char put_no_loc(const char * map_, const char * name,
  const char * map_name, CMAP_PART_CTX * ctx)
{
  put_upd_prev(map_, name, map_name, ctx);

  if(map_ != NULL) return (1 == 0);

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

static const char * map(const char * name, CMAP_PART_CTX_C * c)
{
  const char * map_ = cmap_part_kv_public.get(c -> name2map, NULL, name);
  if(map_ != NULL) return map_;

  if(c -> prev == NULL) return NULL;
  map_ = map(name, c -> prev);
  if(map_ == NULL) return NULL;

  cmap_part_kv_public.put(&c -> name2map, NULL, name, map_);
  cmap_strings_public.add(&c -> params, name);

  return map_;
}

static CMAP_PART_VAR_RET get(const char * map_, const char * name,
  CMAP_PART_CTX * ctx)
{
  CMAP_PART_VAR_RET ret;

  CMAP_PART_CTX_C * c = ctx -> block.c;
  CMAP_PART_KV * name2map = c -> name2map;

  ret.is_def = (map_ == NULL) ? is_def(name, c -> cmap) : (1 == 0);

  ret.map = cmap_part_kv_public.get(name2map, map_, name);
  if(ret.map != NULL)
  {
    ret.dirty = cmap_part_kv_public.is_dirty_n_rst(name2map, map_, name);
    if(ret.dirty) cmap_part_dirty_public.name(map_, name, ctx);
  }
  else if(map_ == NULL)
  {
    ret.map = map(name, c);
    ret.dirty = (1 == 0);
  }

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
  put, put_loc, put_no_loc,
  get,
  defs
};
