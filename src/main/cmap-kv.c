
#include "cmap-kv.h"

#include <stdlib.h>
#include <string.h>

/*******************************************************************************
*******************************************************************************/

struct KV
{
  const char * key, * val;
  KV * next;
};

/*******************************************************************************
*******************************************************************************/

KV * cmap_kv_create()
{
  KV * kv = (KV *)malloc(sizeof(KV));
  kv -> key = NULL;
  kv -> val = NULL;
  kv -> next = NULL;
  return kv;
}

/*******************************************************************************
*******************************************************************************/

void cmap_kv_put(KV * kv, const char * key, const char * val)
{
  KV ** kv_ptr = &kv;

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

  *kv_ptr = (KV *)malloc(sizeof(KV));
  (*kv_ptr) -> key = strdup(key);
  (*kv_ptr) -> val = strdup(val);
  (*kv_ptr) -> next = NULL;
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_kv_get(KV * kv, const char * key)
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

void cmap_kv_delete(KV * kv)
{
  while(kv != NULL)
  {
    KV * tmp = kv;
    kv = kv -> next;

    if(tmp -> key != NULL)
    {
      free((void *)(tmp -> key));
      free((void *)(tmp -> val));
    }
    free(tmp);
  }
}
