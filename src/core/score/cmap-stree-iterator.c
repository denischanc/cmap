
#include "cmap-stree-iterator.h"

#include <stdlib.h>
#include "cmap-mem.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_STREE_NODE ** stree, * cur, * prev;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static char has_next(CMAP_ITERATOR_STREE * this)
{
  return (((INTERNAL *)(this + 1)) -> cur != NULL);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STREE_NODE * next(CMAP_ITERATOR_STREE * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  CMAP_STREE_NODE * cur = internal -> cur;
  internal -> prev = cur;
  if(cur != NULL) internal -> cur = cmap_stree_public.next(cur);
  return cur;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STREE_NODE * get(CMAP_ITERATOR_STREE * this)
{
  return ((INTERNAL *)(this + 1)) -> prev;
}

/*******************************************************************************
*******************************************************************************/

static void rm(CMAP_ITERATOR_STREE * this)
{
  INTERNAL * internal = (INTERNAL *)(this + 1);
  CMAP_STREE_NODE * prev = internal -> prev;
  if(prev != NULL)
  {
    internal -> prev = NULL;
    cmap_stree_public.rm(internal -> stree, prev);
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
  internal -> cur = cmap_stree_public.first(*stree);
  internal -> prev = NULL;

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
