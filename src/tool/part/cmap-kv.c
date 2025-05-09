
#include "cmap-kv.h"

#include <stdlib.h>
#include <string.h>

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char * key, * val, dirty;
} KV;

/*******************************************************************************
*******************************************************************************/

CMAP_STACK_IMPL(kv, KV)

/*******************************************************************************
*******************************************************************************/

static void put(CMAP_KV ** kv_ptr, const char * key, const char * val)
{
  CMAP_KV * kv = *kv_ptr;
  while(kv != NULL)
  {
    KV * kv_elt = &(kv -> v);
    if(!strcmp(key, kv_elt -> key))
    {
      free(kv_elt -> val);
      kv_elt -> val = strdup(val);
      return;
    }
    kv = kv -> next;
  }

  KV kv_elt = {strdup(key), strdup(val), (1 == 0)};
  cmap_stack_kv_push(kv_ptr, kv_elt);
}

/*******************************************************************************
*******************************************************************************/

static const char * get(CMAP_KV * kv, const char * key)
{
  while(kv != NULL)
  {
    if(!strcmp(key, kv -> v.key)) return kv -> v.val;
    kv = kv -> next;
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static void dirty(CMAP_KV * kv, const char * key)
{
  while(kv != NULL)
  {
    if(!strcmp(key, kv -> v.key))
    {
      kv -> v.dirty = (1 == 1);
      return;
    }
    kv = kv -> next;
  }
}

/*******************************************************************************
*******************************************************************************/

static char is_dirty_n_rst(CMAP_KV * kv, const char * key)
{
  while(kv != NULL)
  {
    if(!strcmp(key, kv -> v.key))
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

static void delete_key(CMAP_KV ** kv_ptr, const char * key)
{
  while(*kv_ptr != NULL)
  {
    if(!strcmp((*kv_ptr) -> v.key, key))
    {
      CMAP_KV * cur = *kv_ptr;
      *kv_ptr = cur -> next;
      free(cur -> v.key);
      free(cur -> v.val);
      free(cur);
      return;
    }

    kv_ptr = &(*kv_ptr) -> next;
  }
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_KV ** kv_ptr)
{
  while(*kv_ptr != NULL)
  {
    KV kv_elt = cmap_stack_kv_pop(kv_ptr);
    free(kv_elt.key);
    free(kv_elt.val);
  }
}

/*******************************************************************************
*******************************************************************************/

const CMAP_KV_PUBLIC cmap_kv_public =
{
  put, get,
  dirty, is_dirty_n_rst,
  delete_key, delete
};
