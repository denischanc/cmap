
#include "cmap-part-keys.h"

#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static char contains(CMAP_PART_KEYS * keys, const char * map,
  const char * name)
{
  return (cmap_part_kv_public.get(keys, map, name) != NULL);
}

/*******************************************************************************
*******************************************************************************/

static void add(CMAP_PART_KEYS ** keys, const char * map, const char * name)
{
  if(!contains(*keys, map, name)) cmap_part_kv_public.put(keys, map, name, "");
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_PART_KEYS_APPLY fn;
  void * data;
} APPLY_APPLY_DATA;

static void apply_apply(const char * map, const char * name,
  const char * map_name, void * data)
{
  APPLY_APPLY_DATA * data_ = data;
  data_ -> fn(map, name, data_ -> data);
}

static void apply(CMAP_PART_KEYS * keys, CMAP_PART_KEYS_APPLY fn, void * data)
{
  APPLY_APPLY_DATA data_ = {fn, data};
  cmap_part_kv_public.apply(keys, apply_apply, &data_);
}

/*******************************************************************************
*******************************************************************************/

static void add_all_apply(const char * map, const char * name, void * data)
{
  CMAP_PART_KEYS ** keys = data;
  add(keys, map, name);
}

static void add_all(CMAP_PART_KEYS ** keys, CMAP_PART_KEYS * others)
{
  apply(others, add_all_apply, keys);
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_PART_KEYS ** keys)
{
  cmap_part_kv_public.delete(keys);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_KEYS_PUBLIC cmap_part_keys_public = {contains, add, apply,
  add_all, delete};
