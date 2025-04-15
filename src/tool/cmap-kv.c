
#include "cmap-kv.h"

#include <stdlib.h>
#include <string.h>

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char * key, * val;
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

  KV kv_elt = { strdup(key), strdup(val) };
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
  put,
  get,
  delete
};
