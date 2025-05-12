
#include "cmap-part-keys.h"

#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static void add(CMAP_PART_KEYS ** keys, const char * map, const char * name)
{
  if(cmap_part_kv_public.get(*keys, map, name) == NULL)
    cmap_part_kv_public.put(keys, map, name, "");
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_PART_KEYS_APPLY fn;
  void * data;
} APPLY_APPLY_DATA;

static void apply_apply(const char * map, const char * name,
  const char * map_name, char dirty, void * data)
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

static void delete(CMAP_PART_KEYS ** keys)
{
  cmap_part_kv_public.delete(keys);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_KEYS_PUBLIC cmap_part_keys_public = {add, apply, delete};
