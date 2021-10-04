
#include "cmap-tree.h"

#include <stdlib.h>
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

void * cmap_tree_find(CMAP_TREE_RUNNER * runner, void * tree, void * data)
{
  void * result = NULL;

  while(tree != NULL)
  {
    int v = CMAP_CALL_ARGS(runner, eval, tree, data);
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

void cmap_tree_add(CMAP_TREE_RUNNER * runner, void ** tree, void * node,
  void * data)
{
  *CMAP_CALL_ARGS(runner, ge, node) = NULL;
  *CMAP_CALL_ARGS(runner, lt, node) = NULL;

  void * parent = NULL;

  while(*tree != NULL)
  {
    parent = *tree;

    int v = CMAP_CALL_ARGS(runner, eval, parent, data);
    if(v > 0) tree = CMAP_CALL_ARGS(runner, lt, parent);
    else if(v < 0) tree = CMAP_CALL_ARGS(runner, ge, parent);
    else
    {
      tree = CMAP_CALL_ARGS(runner, ge, parent);
      if(*tree != NULL)
      {
        *CMAP_CALL_ARGS(runner, ge, node) = *tree;
        *CMAP_CALL_ARGS(runner, parent, *tree) = node;
        *tree = NULL;
      }
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

/*******************************************************************************
*******************************************************************************/

void cmap_tree_apply(CMAP_TREE_RUNNER * runner, void ** tree,
  CMAP_TREE_APPLY * apply, char ge_first, void * data)
{
  void * _tree = *tree;
  if(_tree != NULL)
  {
    if(apply -> before != NULL) CMAP_CALL_ARGS(apply, before, tree, data);

    void ** ge = CMAP_CALL_ARGS(runner, ge, _tree);
    void ** lt = CMAP_CALL_ARGS(runner, lt, _tree);
    void ** next = ge_first ? ge : lt;
    cmap_tree_apply(runner, next, apply, ge_first, data);

    if(apply -> between != NULL) CMAP_CALL_ARGS(apply, between, tree, data);

    next = ge_first ? lt : ge;
    cmap_tree_apply(runner, next, apply, ge_first, data);

    if(apply -> after != NULL) CMAP_CALL_ARGS(apply, after, tree, data);
  }
}
