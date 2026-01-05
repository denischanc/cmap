#ifndef __CMAP_STREE_ITERATOR_H__
#define __CMAP_STREE_ITERATOR_H__

#include "cmap-stree.h"
#include "cmap-iterator-define.h"

CMAP_ITERATOR_DECL(STREE, CMAP_STREE_NODE *)

typedef struct
{
  CMAP_ITERATOR_STREE * (*create)(CMAP_STREE_NODE ** stree);
  void (*delete)(CMAP_ITERATOR_STREE * this);

  char (*has_next)(CMAP_ITERATOR_STREE * this);
  CMAP_STREE_NODE * (*next)(CMAP_ITERATOR_STREE * this);
  CMAP_STREE_NODE * (*get)(CMAP_ITERATOR_STREE * this);
  void (*rm)(CMAP_ITERATOR_STREE * this);
} CMAP_STREE_ITERATOR_PUBLIC;

extern const CMAP_STREE_ITERATOR_PUBLIC cmap_stree_iterator_public;

#endif
