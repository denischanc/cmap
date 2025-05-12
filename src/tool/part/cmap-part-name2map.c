
#include "cmap-part-name2map.h"

#include <stdlib.h>
#include "cmap-part-kv.h"
#include "cmap-part-affected.h"
#include "cmap-part-keys.h"

/*******************************************************************************
*******************************************************************************/

static void put(const char * map, const char * name, const char * map_name,
  CMAP_PART_CTX * ctx)
{
  CMAP_PART_CTX_C * c = ctx -> block.c;
  cmap_part_kv_public.put(&c -> name2map, map, name, map_name);
  cmap_part_affected_public.add(map, name, &ctx -> block);

  c = c -> prev;
  while(c != NULL)
  {
    cmap_part_kv_public.delete_key(&c -> name2map, map, name);
    c = c -> prev;
  }
}

/*******************************************************************************
*******************************************************************************/

static const char * get_map(const char * map, const char * name,
  CMAP_PART_CTX_C * c)
{
  const char * map_name = cmap_part_kv_public.get(c -> name2map, map, name);
  if(map_name != NULL) return map_name;

  if(c -> prev == NULL) return NULL;
  map_name = get_map(map, name, c -> prev);
  if(map_name == NULL) return NULL;

  cmap_part_keys_public.add(&c -> params, map, name);
  cmap_part_kv_public.put(&c -> name2map, map, name, map_name);
  cmap_part_affected_public.add(map, name, c -> block);

  return map_name;
}

static CMAP_PART_NAME2MAP_RET get(const char * map, const char * name,
  const char * next_name, CMAP_PART_CTX * ctx)
{
  CMAP_PART_NAME2MAP_RET ret;

  CMAP_PART_CTX_C * c = ctx -> block.c;

  const char * map_name = cmap_part_kv_public.get(c -> name2map, map, name);
  if(map_name != NULL)
  {
    ret.map = map_name;
    ret.affected =
      cmap_part_affected_public.contains_n_add(map, name, &ctx -> block);
  }
  else
  {
    map_name = get_map(map, name, c);
    if(map_name != NULL)
    {
      ret.map = map_name;
      ret.affected = (1 == 1);
    }
    else
    {
      cmap_part_kv_public.put(&c -> name2map, map, name, next_name);
      cmap_part_affected_public.add(map, name, &ctx -> block);

      ret.map = next_name;
    }
  }

  return ret;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_NAME2MAP_PUBLIC cmap_part_name2map_public = {put, get};
