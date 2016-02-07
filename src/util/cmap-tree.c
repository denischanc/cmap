
#include "cmap-tree.h"

#include <stdlib.h>
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

void * cmap_tree_find(CMAP_TREE_RUNNER * runner, void * tree)
{
  void * result = NULL;

  while(tree != NULL)
  {
    int v = CMAP_CALL_ARGS(runner, eval, tree);
    if(v == 0)
    {
      result = tree;
      tree = NULL;
    }
    else if(v > 0)
    {
      if(CMAP_CALL(runner, gt_usable)) result = tree;
      tree = *(CMAP_CALL_ARGS(runner, lt, tree));
    }
    else
    {
      if(CMAP_CALL(runner, lt_usable)) result = tree;
      tree = *(CMAP_CALL_ARGS(runner, ge, tree));
    }
  }

  return result;
}

/*******************************************************************************
*******************************************************************************/

void cmap_tree_add(CMAP_TREE_RUNNER * runner, void ** tree, void * node)
{
  *CMAP_CALL_ARGS(runner, ge, node) = NULL;
  *CMAP_CALL_ARGS(runner, lt, node) = NULL;

  void * parent = NULL;

  while(*tree != NULL)
  {
    parent = *tree;

    int v = CMAP_CALL_ARGS(runner, eval, parent);
    if(v > 0) tree = CMAP_CALL_ARGS(runner, lt, parent);
    else if(v < 0) tree = CMAP_CALL_ARGS(runner, ge, parent);
    else
    {
      void ** ge = CMAP_CALL_ARGS(runner, ge, node);
      *ge = parent;
      parent = *CMAP_CALL_ARGS(runner, parent, parent);
      *CMAP_CALL_ARGS(runner, parent, *ge) = node;

      *tree = NULL;
    }
  }

  *tree = node;
  *CMAP_CALL_ARGS(runner, parent, node) = parent;
}

/*******************************************************************************
*******************************************************************************/

void cmap_tree_rm(CMAP_TREE_RUNNER * runner, void ** tree, void * node)
{
  void * repl, * parent;

  void ** ge = CMAP_CALL_ARGS(runner, ge, node);
  void ** lt = CMAP_CALL_ARGS(runner, lt, node);
  if(*ge == NULL) repl = *lt;
  else if(*lt == NULL) repl = *ge;
  else
  {
    repl = *ge;

    parent = repl;
    void ** cur_ptr = CMAP_CALL_ARGS(runner, lt, repl);
    while(*cur_ptr != NULL)
    {
      parent = *cur_ptr;
      cur_ptr = CMAP_CALL_ARGS(runner, lt, parent);
    }
    *cur_ptr = *lt;
    *CMAP_CALL_ARGS(runner, parent, *cur_ptr) = parent;
  }

  parent = *CMAP_CALL_ARGS(runner, parent, node);
  if(repl != NULL) *CMAP_CALL_ARGS(runner, parent, repl) = parent;

  if(parent == NULL) *tree = repl;
  else
  {
    void ** ge = CMAP_CALL_ARGS(runner, ge, parent);
    if(*ge == node) *ge = repl;
    else *CMAP_CALL_ARGS(runner, lt, parent) = repl;
  }
}

/*******************************************************************************
*******************************************************************************/

char cmap_tree_usable_true(CMAP_TREE_RUNNER * this)
{
  return CMAP_T;
}

char cmap_tree_usable_false(CMAP_TREE_RUNNER * this)
{
  return CMAP_F;
}
