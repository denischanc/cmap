
#include "cmap-stree-iterator.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-mem.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_STREE_NODE ** stree, * cur, * next;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static char has_next(CMAP_ITERATOR_STREE * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  if(internal -> stree == NULL) return CMAP_F;
  if(internal -> next != NULL) return CMAP_T;

  if(internal -> cur == NULL)
  {
    CMAP_STREE_NODE * stree = *internal -> stree;
    if(stree == NULL)
    {
      internal -> stree = NULL;
      return CMAP_F;
    }
    internal -> next = cmap_stree_public.first(stree);
    return CMAP_T;
  }

  internal -> next = cmap_stree_public.next(internal -> cur);
  if(internal -> next == NULL)
  {
    internal -> stree = NULL;
    return CMAP_F;
  }
  return CMAP_T;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STREE_NODE * next(CMAP_ITERATOR_STREE * this)
{
  if(!has_next(this)) return NULL;

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> cur = internal -> next;
  internal -> next = NULL;
  return internal -> cur;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STREE_NODE * get(CMAP_ITERATOR_STREE * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  return (internal -> stree == NULL) ? NULL : internal -> cur;
}

/*******************************************************************************
*******************************************************************************/

static void rm(CMAP_ITERATOR_STREE * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  CMAP_STREE_NODE ** stree = internal -> stree, * cur = internal -> cur;
  if((stree != NULL) && (cur != NULL))
  {
    has_next(this);
    cmap_stree_public.rm(stree, cur);
    internal -> cur = NULL;
  }
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_ITERATOR_STREE * this)
{
  CMAP_MEM_INSTANCE_FREE(this);
}

static CMAP_ITERATOR_STREE * create(CMAP_STREE_NODE ** stree)
{
  CMAP_MEM_VAR;
  CMAP_ITERATOR_STREE * this = mem -> alloc(sizeof(CMAP_ITERATOR_STREE) +
    sizeof(INTERNAL));

  INTERNAL * internal = (INTERNAL *)(this + 1);
  internal -> stree = stree;
  internal -> cur = NULL;
  internal -> next = NULL;

  this -> delete = delete;
  this -> has_next = has_next;
  this -> next = next;
  this -> get = get;
  this -> rm = rm;

  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_STREE_ITERATOR_PUBLIC cmap_stree_iterator_public =
{
  create, delete,
  has_next, next, get, rm
};
