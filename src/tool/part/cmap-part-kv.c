
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
  char * map, dirty;
} KV;

/*******************************************************************************
*******************************************************************************/

CMAP_STACK_IMPL(PART_KV, part_kv, KV)

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
  while(kv != NULL)
  {
    KV * kv_elt = &kv -> v;
    if(is_eq(&kv_elt -> key, map, name))
    {
      free(kv_elt -> map);
      kv_elt -> map = strdup(map_name);
      return;
    }
    kv = kv -> next;
  }

  KV kv_elt = {{NULL, strdup(name)}, strdup(map_name), (1 == 0)};
  if(map != NULL) kv_elt.key.map = strdup(map);
  cmap_stack_part_kv_push(kv_ptr, kv_elt);
}

/*******************************************************************************
*******************************************************************************/

static const char * get(CMAP_PART_KV * kv, const char * map, const char * name)
{
  while(kv != NULL)
  {
    if(is_eq(&kv -> v.key, map, name)) return kv -> v.map;
    kv = kv -> next;
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static void dirty(CMAP_PART_KV * kv, const char * map, const char * name)
{
  while(kv != NULL)
  {
    if(is_eq(&kv -> v.key, map, name))
    {
      kv -> v.dirty = (1 == 1);
      return;
    }
    kv = kv -> next;
  }
}

/*******************************************************************************
*******************************************************************************/

static char is_dirty_n_rst(CMAP_PART_KV * kv, const char * map,
  const char * name)
{
  while(kv != NULL)
  {
    if(is_eq(&kv -> v.key, map, name))
    {
      char ret = kv -> v.dirty;
      kv -> v.dirty = (1 == 0);
      return ret;
    }
    kv = kv -> next;
  }
  return (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

static void apply(CMAP_PART_KV * kv, CMAP_PART_KV_APPLY fn, void * data)
{
  while(kv != NULL)
  {
    KV * kv_elt = &kv -> v;
    fn(kv_elt -> key.map, kv_elt -> key.name, kv_elt -> map, kv_elt -> dirty,
      data);

    kv = kv -> next;
  }
}

/*******************************************************************************
*******************************************************************************/

static void delete_key(CMAP_PART_KV ** kv_ptr, const char * map,
  const char * name)
{
  while(*kv_ptr != NULL)
  {
    if(is_eq(&(*kv_ptr) -> v.key, map, name))
    {
      CMAP_PART_KV * cur = *kv_ptr;
      *kv_ptr = cur -> next;
      free(cur -> v.key.map);
      free(cur -> v.key.name);
      free(cur -> v.map);
      free(cur);
      return;
    }

    kv_ptr = &(*kv_ptr) -> next;
  }
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_PART_KV ** kv_ptr)
{
  while(*kv_ptr != NULL)
  {
    KV kv_elt = cmap_stack_part_kv_pop(kv_ptr);
    free(kv_elt.key.map);
    free(kv_elt.key.name);
    free(kv_elt.map);
  }
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_KV_PUBLIC cmap_part_kv_public =
{
  put, get,
  dirty, is_dirty_n_rst,
  apply,
  delete_key, delete
};
