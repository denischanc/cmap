
#include "cmap-part-kv.h"

#include <stdlib.h>
#include <string.h>

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char * map, * name;
} KEY;

typedef struct
{
  KEY key;
  char * map;
} KV;

/*******************************************************************************
*******************************************************************************/

static KV create_kv(const char * map, const char * name, const char * map_name)
{
  KV kv;
  kv.key.map = (map == NULL) ? NULL : strdup(map);
  kv.key.name = strdup(name);
  kv.map = strdup(map_name);
  return kv;
}

static void free_kv(KV * kv)
{
  free(kv -> key.map);
  free(kv -> key.name);
  free(kv -> map);
}

/*******************************************************************************
*******************************************************************************/

CMAP_STACK_IMPL(PART_KV, part_kv, KV)

static void rm_kv(CMAP_PART_KV ** kv_ptr)
{
  KV kv_elt = cmap_stack_part_kv_pop(kv_ptr);
  free_kv(&kv_elt);
}

/*******************************************************************************
*******************************************************************************/

static char is_eq(KEY * key, const char * map, const char * name)
{
  return (!strcmp(key -> name, name) &&
    (((key -> map == NULL) && (map == NULL)) ||
     ((key -> map != NULL) && (map != NULL) && !strcmp(key -> map, map))));
}

/*******************************************************************************
*******************************************************************************/

static void put(CMAP_PART_KV ** kv_ptr, const char * map, const char * name,
  const char * map_name)
{
  CMAP_PART_KV * kv = *kv_ptr;
  if(kv == NULL)
    cmap_stack_part_kv_push(kv_ptr, create_kv(map, name, map_name));
  else
  {
    KV * kv_elt = &kv -> v;
    if(is_eq(&kv_elt -> key, map, name))
    {
      free(kv_elt -> map);
      kv_elt -> map = strdup(map_name);
    }
    else put(&kv -> next, map, name, map_name);
  }
}

/*******************************************************************************
*******************************************************************************/

static const char * get(CMAP_PART_KV * kv, const char * map, const char * name)
{
  if(kv == NULL) return NULL;

  if(is_eq(&kv -> v.key, map, name)) return kv -> v.map;

  return get(kv -> next, map, name);
}

/*******************************************************************************
*******************************************************************************/

static void apply(CMAP_PART_KV ** kv_ptr, CMAP_PART_KV_APPLY fn, void * data)
{
  CMAP_PART_KV * kv = *kv_ptr;
  if(kv == NULL) return;

  KV * kv_elt = &kv -> v;
  if(fn(kv_elt -> key.map, kv_elt -> key.name, kv_elt -> map, data))
    rm_kv(kv_ptr);
  else kv_ptr = &kv -> next;

  apply(kv_ptr, fn, data);
}

/*******************************************************************************
*******************************************************************************/

static void delete_key(CMAP_PART_KV ** kv_ptr, const char * map,
  const char * name)
{
  CMAP_PART_KV * kv = *kv_ptr;
  if(kv == NULL) return;

  if(is_eq(&kv -> v.key, map, name)) rm_kv(kv_ptr);
  else delete_key(&kv -> next, map, name);
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_PART_KV ** kv_ptr)
{
  if(*kv_ptr == NULL) return;

  rm_kv(kv_ptr);

  delete(kv_ptr);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_KV_PUBLIC cmap_part_kv_public =
{
  put, get,
  apply,
  delete_key, delete
};
