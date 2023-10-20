
#include "cmap-kv.h"

#include <stdlib.h>
#include <string.h>

/*******************************************************************************
*******************************************************************************/

struct CMAP_KV
{
  const char * key, * val;
  CMAP_KV * next;
};

/*******************************************************************************
*******************************************************************************/

CMAP_KV * create()
{
  CMAP_KV * kv = (CMAP_KV *)malloc(sizeof(CMAP_KV));
  kv -> key = NULL;
  kv -> val = NULL;
  kv -> next = NULL;
  return kv;
}

/*******************************************************************************
*******************************************************************************/

void put(CMAP_KV * kv, const char * key, const char * val)
{
  CMAP_KV ** kv_ptr = &kv;

  while(*kv_ptr != NULL)
  {
    if((*kv_ptr) -> key == NULL)
    {
      (*kv_ptr) -> key = strdup(key);
      (*kv_ptr) -> val = strdup(val);
      return;
    }
    else if(!strcmp(key, (*kv_ptr) -> key))
    {
      free((void *)((*kv_ptr) -> val));
      (*kv_ptr) -> val = strdup(val);
      return;
    }

    kv_ptr = &(*kv_ptr) -> next;
  }

  *kv_ptr = (CMAP_KV *)malloc(sizeof(CMAP_KV));
  (*kv_ptr) -> key = strdup(key);
  (*kv_ptr) -> val = strdup(val);
  (*kv_ptr) -> next = NULL;
}

/*******************************************************************************
*******************************************************************************/

const char * get(CMAP_KV * kv, const char * key)
{
  while(kv != NULL)
  {
    if((kv -> key != NULL) && !strcmp(kv -> key, key)) return kv -> val;
    kv = kv -> next;
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

void delete(CMAP_KV * kv)
{
  while(kv != NULL)
  {
    CMAP_KV * tmp = kv;
    kv = kv -> next;

    if(tmp -> key != NULL)
    {
      free((void *)(tmp -> key));
      free((void *)(tmp -> val));
    }
    free(tmp);
  }
}

/*******************************************************************************
*******************************************************************************/

const CMAP_KV_PUBLIC cmap_kv_public =
{
  create,
  put,
  get,
  delete
};
