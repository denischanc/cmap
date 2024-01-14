
#include "cmap-tree.h"

#include <stdlib.h>
#include <sys/param.h>
#include "cmap.h"

/*******************************************************************************
*******************************************************************************/

#define NODE(runner, node_) runner -> node(node_)

/*******************************************************************************
*******************************************************************************/

static void * find(CMAP_TREE_RUNNER * runner, void * tree, void * data)
{
  void * result = NULL;

  while(tree != NULL)
  {
    int v = runner -> eval(tree, data);
    if(v == 0)
    {
      result = tree;
      tree = NULL;
    }
    else if(v > 0)
    {
      if(runner -> gt_usable) result = tree;
      tree = NODE(runner, tree) -> lt;
    }
    else
    {
      if(runner -> lt_usable) result = tree;
      tree = NODE(runner, tree) -> gt;
    }
  }

  return result;
}

/*******************************************************************************
*******************************************************************************/

static inline void update_gt_lt(CMAP_TREE_RUNNER * runner, void * node,
  CMAP_TREE_NODE * node_node, CMAP_TREE_NODE * old_node_node)
{
  void * gt = old_node_node -> gt;
  node_node -> gt = gt;
  if(gt != NULL) NODE(runner, gt) -> parent = node;

  void * lt = old_node_node -> lt;
  node_node -> lt = lt;
  if(lt != NULL) NODE(runner, lt) -> parent = node;
}

static inline void update_eq(CMAP_TREE_RUNNER * runner, void * node,
  CMAP_TREE_NODE * node_node, CMAP_TREE_NODE * old_node_node)
{
  void * eq = old_node_node -> eq;
  node_node -> eq = eq;
  if(eq != NULL) NODE(runner, eq) -> parent = node;
}

/*******************************************************************************
*******************************************************************************/

static inline void init_node(CMAP_TREE_NODE * node_node, void * parent)
{
  node_node -> gt = NULL;
  node_node -> eq = NULL;
  node_node -> lt = NULL;
  node_node -> parent = parent;
  node_node -> depth = 0;
  node_node -> size = 1;
}

/*******************************************************************************
*******************************************************************************/

static inline int depth(CMAP_TREE_RUNNER * runner, CMAP_TREE_NODE * node_node)
{
  void * gt = node_node -> gt, * lt = node_node -> lt;

  if(gt == NULL)
  {
    if(lt == NULL) return 0;
    else return (1 + NODE(runner, lt) -> depth);
  }
  else
  {
    if(lt == NULL) return (1 + NODE(runner, gt) -> depth);
    else
    {
      int depth_gt = NODE(runner, gt) -> depth,
        depth_lt = NODE(runner, lt) -> depth;
      return (1 + MAX(depth_gt, depth_lt));
    }
  }
}

/*******************************************************************************
*******************************************************************************/

static inline void step_size_until(char step, void * stop,
  CMAP_TREE_RUNNER * runner, void * from)
{
  char depth_updated = CMAP_T;
  while(from != NULL)
  {
    CMAP_TREE_NODE * from_node = NODE(runner, from);
    from_node -> size += step;

    if(depth_updated)
    {
      int old_depth = from_node -> depth;
      from_node -> depth = depth(runner, from_node);
      depth_updated = (old_depth != from_node -> depth);
    }

    if(from == stop) return;
    else from = from_node -> parent;
  }
}

/*******************************************************************************
*******************************************************************************/

static inline void inc_size_until(void * stop, CMAP_TREE_RUNNER * runner,
  void * from)
{
  step_size_until(1, stop, runner, from);
}

static inline void dec_size_until(void * stop, CMAP_TREE_RUNNER * runner,
  void * from)
{
  step_size_until(-1, stop, runner, from);
}

/*******************************************************************************
*******************************************************************************/

static inline void add_eq(CMAP_TREE_RUNNER * runner, void * node, void * parent)
{
  CMAP_TREE_NODE * node_node = NODE(runner, node),
    * parent_node = NODE(runner, parent);

  node_node -> gt = NULL;
  update_eq(runner, node, node_node, parent_node);
  node_node -> lt = NULL;
  node_node -> parent = parent;

  parent_node -> eq = node;
}

/*******************************************************************************
*******************************************************************************/

static void add(CMAP_TREE_RUNNER * runner, void ** tree, void * node,
  void * data)
{
  void * parent = NULL;
  while(*tree != NULL)
  {
    parent = *tree;

    int v = runner -> eval(parent, data);
    if(v > 0) tree = &NODE(runner, parent) -> lt;
    else if(v < 0) tree = &NODE(runner, parent) -> gt;
    else
    {
      add_eq(runner, node, parent);
      return;
    }
  }

  init_node(NODE(runner, node), parent);
  *tree = node;
  inc_size_until(NULL, runner, parent);
}

/*******************************************************************************
*******************************************************************************/

static void * take_last_lt(CMAP_TREE_RUNNER * runner, void * node)
{
  void * lt = node;
  CMAP_TREE_NODE * lt_node = NODE(runner, lt);

  void * next = lt_node -> lt;
  char first = CMAP_T;
  while(next != NULL)
  {
    first = CMAP_F;
    lt = next;
    lt_node = NODE(runner, lt);
    next = lt_node -> lt;
  }

  void * gt = lt_node -> gt, * parent = lt_node -> parent;
  CMAP_TREE_NODE * parent_node = NODE(runner, parent);
  if(first) parent_node -> gt = gt; else parent_node -> lt = gt;
  if(gt != NULL) NODE(runner, gt) -> parent = parent;

  if(lt != node) dec_size_until(node, runner, parent);

  return lt;
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_eq(CMAP_TREE_RUNNER * runner, CMAP_TREE_NODE * node_node,
  void * parent, CMAP_TREE_NODE * parent_node)
{
  void * eq = node_node -> eq;
  if(eq != NULL)
  {
    CMAP_TREE_NODE * eq_node = NODE(runner, eq);
    eq_node -> parent = parent;
  }
  parent_node -> eq = eq;
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_first_eq(CMAP_TREE_RUNNER * runner, void ** tree,
  void * node, CMAP_TREE_NODE * node_node, CMAP_TREE_NODE * parent_node)
{
  void * eq = node_node -> eq;
  CMAP_TREE_NODE * eq_node = NODE(runner, eq);
  update_gt_lt(runner, eq, eq_node, node_node);
  eq_node -> parent = node_node -> parent;
  eq_node -> size = node_node -> size;
  eq_node -> depth = node_node -> depth;

  if(parent_node == NULL) *tree = eq;
  else
  {
    if(parent_node -> gt == node) parent_node -> gt = eq;
    else parent_node -> lt = eq;
  }
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_not_eq(CMAP_TREE_RUNNER * runner, void ** tree,
  void * node, CMAP_TREE_NODE * node_node, void * parent,
  CMAP_TREE_NODE * parent_node)
{
  void * repl = NULL, * gt = node_node -> gt, * lt = node_node -> lt;
  CMAP_TREE_NODE * repl_node = NULL;

  if(gt == NULL)
  {
    if(lt != NULL) { repl = lt; repl_node = NODE(runner, repl); }
  }
  else if(lt == NULL) { repl = gt; repl_node = NODE(runner, repl); }
  else
  {
    repl = take_last_lt(runner, gt); repl_node = NODE(runner, repl);

    update_gt_lt(runner, repl, repl_node, node_node);
    repl_node -> size = node_node -> size - 1;
    repl_node -> depth = depth(runner, repl_node);
  }

  if(repl_node != NULL) repl_node -> parent = parent;
  if(parent == NULL) *tree = repl;
  else
  {
    if(parent_node -> gt == node) parent_node -> gt = repl;
    else parent_node -> lt = repl;

    dec_size_until(*tree, runner, parent);
  }
}

/*******************************************************************************
*******************************************************************************/

static void rm(CMAP_TREE_RUNNER * runner, void ** tree, void * node)
{
  CMAP_TREE_NODE * node_node = NODE(runner, node), * parent_node = NULL;
  void * parent = node_node -> parent;

  if(parent != NULL) parent_node = NODE(runner, parent);

  if((parent_node != NULL) && (parent_node -> eq == node))
  {
    rm_eq(runner, node_node, parent, parent_node);
  }
  else if(node_node -> eq != NULL)
  {
    rm_first_eq(runner, tree, node, node_node, parent_node);
  }
  else
  {
    rm_not_eq(runner, tree, node, node_node, parent, parent_node);
  }
}

/*******************************************************************************
*******************************************************************************/

static void apply(CMAP_TREE_RUNNER * runner, void * tree,
  CMAP_TREE_APPLY * apply_, char gt_first, void * data)
{
  if(tree != NULL)
  {
    CMAP_TREE_NODE * tree_node = NODE(runner, tree);
    void * gt = tree_node -> gt, * eq = tree_node -> eq, * lt = tree_node -> lt;

    if(apply_ -> before != NULL) apply_ -> before(tree, data);

    apply(runner, gt_first ? gt : lt, apply_, gt_first, data);

    apply(runner, eq, apply_, gt_first, data);

    if(apply_ -> between != NULL) apply_ -> between(tree, data);

    apply(runner, gt_first ? lt : gt, apply_, gt_first, data);

    if(apply_ -> after != NULL) apply_ -> after(tree, data);
  }
}

/*******************************************************************************
*******************************************************************************/

static void clean(CMAP_TREE_RUNNER * runner, void ** tree,
  CMAP_TREE_APPLY_FN clean_node, void * data)
{
  void * node = *tree;
  if(node != NULL)
  {
    CMAP_TREE_NODE * node_node = NODE(runner, node);
    clean(runner, &node_node -> gt, clean_node, data);
    clean(runner, &node_node -> eq, clean_node, data);
    clean(runner, &node_node -> lt, clean_node, data);

    clean_node(node, data);

    *tree = NULL;
  }
}

/*******************************************************************************
*******************************************************************************/

CMAP_TREE_NODE * cmap_tree_node(void * node)
{
  return (CMAP_TREE_NODE *)node;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_TREE_PUBLIC cmap_tree_public =
{
  find,
  add, rm,
  apply,
  clean
};
