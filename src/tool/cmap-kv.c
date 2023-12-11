
#include "cmap-kv.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-stack-define.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char * key, * val;
} KV_CTR;

CMAP_STACK(kv, KV_CTR)

struct CMAP_KV
{
  CMAP_STACK_kv * stack;
};

/*******************************************************************************
*******************************************************************************/

static CMAP_KV * create()
{
  CMAP_KV * kv = (CMAP_KV *)malloc(sizeof(CMAP_KV));
  kv -> stack = NULL;
  return kv;
}

/*******************************************************************************
*******************************************************************************/

static void put(CMAP_KV * kv, const char * key, const char * val)
{
  CMAP_STACK_kv * stack = kv -> stack;
  while(stack != NULL)
  {
    if(!strcmp(key, stack -> v.key))
    {
      free(stack -> v.val);
      stack -> v.val = strdup(val);
      return;
    }
    stack = stack -> next;
  }

  KV_CTR ctr = { strdup(key), strdup(val) };
  cmap_stack_kv_push(&kv -> stack, ctr);
}

/*******************************************************************************
*******************************************************************************/

static const char * get(CMAP_KV * kv, const char * key)
{
  CMAP_STACK_kv * stack = kv -> stack;
  while(stack != NULL)
  {
    if(!strcmp(key, stack -> v.key)) return stack -> v.val;
    stack = stack -> next;
  }
  return NULL;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_KV * kv)
{
  while(kv -> stack != NULL)
  {
    KV_CTR ctr = cmap_stack_kv_pop(&kv -> stack);
    free(ctr.key);
    free(ctr.val);
  }
  free(kv);
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
