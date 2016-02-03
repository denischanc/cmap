
#include "cmap-util-tree.h"

#include <stdlib.h>
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

void * cmap_util_tree_find(CMAP_UTIL_TREE_HANDLER * handler, void * tree)
{
  void * result = NULL;

  while(tree != NULL)
  {
    int v = CMAP_CALL_ARGS(handler, eval, tree);
    if(v == 0)
    {
      result = tree;
      tree = NULL;
    }
    else if(v > 0)
    {
      if(CMAP_CALL(handler, gt_usable)) result = tree;
      tree = *(CMAP_CALL_ARGS(handler, lt, tree));
    }
    else
    {
      if(CMAP_CALL(handler, lt_usable)) result = tree;
      tree = *(CMAP_CALL_ARGS(handler, ge, tree));
    }
  }

  return result;
}

/*******************************************************************************
*******************************************************************************/

void cmap_util_tree_add(CMAP_UTIL_TREE_HANDLER * handler, void ** tree,
  void * node)
{
  void * parent = NULL;

  while(*tree != NULL)
  {
    parent = *tree;

    int v = CMAP_CALL_ARGS(handler, eval, parent);
    if(v > 0) tree = CMAP_CALL_ARGS(handler, lt, parent);
    else if(v < 0) tree = CMAP_CALL_ARGS(handler, ge, parent);
    else
    {
      void ** ge = CMAP_CALL_ARGS(handler, ge, node);
      *ge = parent;
      parent = *CMAP_CALL_ARGS(handler, parent, parent);
      *CMAP_CALL_ARGS(handler, parent, *ge) = node;

      *tree = NULL;
    }
  }

  *tree = node;
  *CMAP_CALL_ARGS(handler, parent, node) = parent;
}

/*******************************************************************************
*******************************************************************************/

void cmap_util_tree_rm(CMAP_UTIL_TREE_HANDLER * handler, void ** tree,
  void * node)
{
  void * repl, * parent;

  void ** ge = CMAP_CALL_ARGS(handler, ge, node);
  void ** lt = CMAP_CALL_ARGS(handler, lt, node);
  if(*ge == NULL) repl = *lt;
  else if(*lt == NULL) repl = *ge;
  else
  {
    repl = *ge;

    parent = repl;
    void ** cur_ptr = CMAP_CALL_ARGS(handler, lt, repl);
    while(*cur_ptr != NULL)
    {
      parent = *cur_ptr;
      cur_ptr = CMAP_CALL_ARGS(handler, lt, parent);
    }
    *cur_ptr = *lt;
    *CMAP_CALL_ARGS(handler, parent, *cur_ptr) = parent;
  }

  parent = *CMAP_CALL_ARGS(handler, parent, node);
  if(repl != NULL) *CMAP_CALL_ARGS(handler, parent, repl) = parent;

  if(parent == NULL) *tree = repl;
  else
  {
    void ** ge = CMAP_CALL_ARGS(handler, ge, parent);
    if(*ge == node) *ge = repl;
    else *CMAP_CALL_ARGS(handler, lt, parent) = repl;
  }
}
