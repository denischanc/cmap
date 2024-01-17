
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
      void * eq = NODE(runner, tree) -> eq;
      result = (eq != NULL) ? eq : tree;
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

#define UPDATE_WAY_IMPL(way) \
static inline void update_##way(CMAP_TREE_RUNNER * runner, void * node, \
  CMAP_TREE_NODE * node_node, CMAP_TREE_NODE * old_node_node) \
{ \
  void * way = old_node_node -> way; \
  node_node -> way = way; \
  if(way != NULL) NODE(runner, way) -> parent = node; \
}

UPDATE_WAY_IMPL(gt)
UPDATE_WAY_IMPL(eq)
UPDATE_WAY_IMPL(lt)

static inline void update_gt_lt(CMAP_TREE_RUNNER * runner, void * node,
  CMAP_TREE_NODE * node_node, CMAP_TREE_NODE * old_node_node)
{
  update_gt(runner, node, node_node, old_node_node);
  update_lt(runner, node, node_node, old_node_node);
}

/*******************************************************************************
*******************************************************************************/

static inline void update_depth_min_max(CMAP_TREE_RUNNER * runner,
  CMAP_TREE_NODE * node_node)
{
  int depth_min = 0, depth_max = 0;

  if(node_node -> gt != NULL)
  {
    CMAP_TREE_NODE * gt_node = NODE(runner, node_node -> gt);
    depth_min = 1 + gt_node -> depth_min;
    depth_max = 1 + gt_node -> depth_max;
  }

  if(node_node -> lt == NULL) depth_min = 0;
  else
  {
    CMAP_TREE_NODE * lt_node = NODE(runner, node_node -> lt);
    int lt_depth_min = 1 + lt_node -> depth_min,
      lt_depth_max = 1 + lt_node -> depth_max;
    if(lt_depth_min < depth_min) depth_min = lt_depth_min;
    if(lt_depth_max > depth_max) depth_max = lt_depth_max;
  }

  node_node -> depth_min = depth_min;
  node_node -> depth_max = depth_max;
}

/*******************************************************************************
*******************************************************************************/

static inline void init_node_wo_eq(CMAP_TREE_NODE * node_node, void * parent)
{
  node_node -> gt = NULL;
  node_node -> lt = NULL;
  node_node -> parent = parent;
  node_node -> depth_min = 0;
  node_node -> depth_max = 0;
}

static inline void init_node(CMAP_TREE_NODE * node_node, void * parent)
{
  init_node_wo_eq(node_node, parent);
  node_node -> eq = NULL;
}

/*******************************************************************************
*******************************************************************************/

static inline void ** parent_(CMAP_TREE_RUNNER * runner, void ** tree,
  CMAP_TREE_NODE * node_node)
{
  void * parent = node_node -> parent;
  if(parent == NULL) return NULL;

  void * parent_parent = NODE(runner, parent) -> parent;
  if(parent_parent == NULL) return tree;
  else
  {
    CMAP_TREE_NODE * parent_parent_node = NODE(runner, parent_parent);
    if(parent_parent_node -> gt == parent) return &parent_parent_node -> gt;
    else return &parent_parent_node -> lt;
  }
}

static inline void ** self_(CMAP_TREE_RUNNER * runner, void ** tree,
  void * node, CMAP_TREE_NODE * node_node)
{
  void * parent = node_node -> parent;
  if(parent == NULL) return tree;

  CMAP_TREE_NODE * parent_node = NODE(runner, parent);
  if(parent_node -> gt == node) return &parent_node -> gt;
  else return &parent_node -> lt;
}

/*******************************************************************************
*******************************************************************************/

#define GET_LAST_WAY_IMPL(way) \
static inline void ** get_last_##way(CMAP_TREE_RUNNER * runner, void ** node) \
{ \
  void ** last = &NODE(runner, *node) -> way; \
  while(*last != NULL) \
  { \
    node = last; \
    last = &NODE(runner, *node) -> way; \
  } \
  return node; \
}

GET_LAST_WAY_IMPL(gt)
GET_LAST_WAY_IMPL(lt)

/*******************************************************************************
*******************************************************************************/

static inline void rebalance_until(void ** stop, CMAP_TREE_RUNNER * runner,
  void ** tree, void ** node);

#define TAKE_LAST_WAY_IMPL(way, other) \
static inline void * take_last_##way(CMAP_TREE_RUNNER * runner, \
  void ** tree, void ** node) \
{ \
  void ** last = get_last_##way(runner, node); \
  void * ret = *last; \
  CMAP_TREE_NODE * ret_node = NODE(runner, ret); \
  void ** parent = parent_(runner, tree, ret_node); \
 \
  void * other = ret_node -> other; \
  if(other != NULL) NODE(runner, other) -> parent = *parent; \
  *last = other; \
 \
  if(last != node) rebalance_until(node, runner, tree, parent); \
 \
  return ret; \
}

TAKE_LAST_WAY_IMPL(gt, lt)
TAKE_LAST_WAY_IMPL(lt, gt)

/*******************************************************************************
*******************************************************************************/

#define PACK_WAY_IMPL(way, other) \
static inline void pack_##way(CMAP_TREE_RUNNER * runner, void ** tree, \
  void ** node, CMAP_TREE_NODE * node_node) \
{ \
  void * repl = take_last_##other(runner, tree, &node_node -> way); \
  CMAP_TREE_NODE * repl_node = NODE(runner, repl); \
  update_gt_lt(runner, repl, repl_node, node_node); \
  repl_node -> parent = node_node -> parent; \
 \
  void * node_bup = *node; \
  *node = repl; \
 \
  void ** other = &repl_node -> other; \
  if(*other == NULL) \
  { \
    repl_node -> other = node_bup; \
    init_node_wo_eq(node_node, repl); \
  } \
  else \
  { \
    void ** last = get_last_##way(runner, other); \
    CMAP_TREE_NODE * last_node = NODE(runner, *last); \
    last_node -> way = node_bup; \
    init_node_wo_eq(node_node, *last); \
    rebalance_until(other, runner, tree, last); \
  } \
 \
  update_depth_min_max(runner, repl_node); \
}

PACK_WAY_IMPL(gt, lt)
PACK_WAY_IMPL(lt, gt)

/*******************************************************************************
*******************************************************************************/

static inline char is_packable(CMAP_TREE_RUNNER * runner, void * node)
{
  if(node == NULL) return CMAP_F;
  else
  {
    CMAP_TREE_NODE * node_node = NODE(runner, node);
    return ((node_node -> depth_max - node_node -> depth_min) > 1);
  }
}

static inline char is_full(CMAP_TREE_RUNNER * runner, void * node)
{
  if(node == NULL) return CMAP_F;
  else
  {
    CMAP_TREE_NODE * node_node = NODE(runner, node);
    return (node_node -> depth_min == node_node -> depth_max);
  }
}

/*******************************************************************************
*******************************************************************************/

static inline void pack_gt_lt(CMAP_TREE_RUNNER * runner, void ** tree,
  void ** node, CMAP_TREE_NODE * node_node)
{
  if(node_node -> gt == NULL) pack_lt(runner, tree, node, node_node);
  else if(node_node -> lt == NULL) pack_gt(runner, tree, node, node_node);
  else
  {
    CMAP_TREE_NODE * gt_node = NODE(runner, node_node -> gt),
      * lt_node = NODE(runner, node_node -> lt);
    if(gt_node -> depth_min > lt_node -> depth_min)
      pack_gt(runner, tree, node, node_node);
    else pack_lt(runner, tree, node, node_node);
  }
}

static void pack(CMAP_TREE_RUNNER * runner, void ** tree, void ** node,
  CMAP_TREE_NODE * node_node)
{
  void ** gt = &node_node -> gt;
  if(is_packable(runner, *gt)) pack(runner, tree, gt, NODE(runner, *gt));
  else
  {
    void ** lt = &node_node -> lt;
    if(is_packable(runner, *lt)) pack(runner, tree, lt, NODE(runner, *lt));
    else pack_gt_lt(runner, tree, node, node_node);
  }
}

/*******************************************************************************
*******************************************************************************/

static inline void rebalance_until(void ** stop, CMAP_TREE_RUNNER * runner,
  void ** tree, void ** node)
{
  char pack_done = CMAP_F;
  while(CMAP_T)
  {
    CMAP_TREE_NODE * node_node = NODE(runner, *node);
    int depth_min = node_node -> depth_min, depth_max = node_node -> depth_max;
    update_depth_min_max(runner, node_node);

    if(!pack_done)
    {
      if(is_packable(runner, *node))
      {
        pack(runner, tree, node, node_node);
        pack_done = CMAP_T;
      }
    }

    node_node = NODE(runner, *node);
    if((node_node -> depth_min == depth_min) &&
      (node_node -> depth_max == depth_max)) return;
    if(node == stop) return;

    node = parent_(runner, tree, node_node);
  }
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
  void ** parent = NULL, ** cur = tree;
  while(*cur != NULL)
  {
    parent = cur;

    int v = runner -> eval(*cur, data);
    if(v > 0) cur = &NODE(runner, *cur) -> lt;
    else if(v < 0) cur = &NODE(runner, *cur) -> gt;
    else
    {
      add_eq(runner, node, *cur);
      return;
    }
  }

  init_node(NODE(runner, node), (parent == NULL) ? NULL : *parent);
  *cur = node;

  if(parent != NULL) rebalance_until(tree, runner, tree, parent);
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_first_eq(CMAP_TREE_RUNNER * runner, void ** tree,
  void * node, CMAP_TREE_NODE * node_node)
{
  void * eq = node_node -> eq;
  CMAP_TREE_NODE * eq_node = NODE(runner, eq);
  update_gt_lt(runner, eq, eq_node, node_node);
  eq_node -> parent = node_node -> parent;
  eq_node -> depth_min = node_node -> depth_min;
  eq_node -> depth_max = node_node -> depth_max;

  *self_(runner, tree, node, node_node) = eq;
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_not_eq(CMAP_TREE_RUNNER * runner, void ** tree,
  void * node, CMAP_TREE_NODE * node_node, void * parent,
  CMAP_TREE_NODE * parent_node)
{
  void * repl = NULL, ** gt = &node_node -> gt, ** lt = &node_node -> lt;
  CMAP_TREE_NODE * repl_node = NULL;

  if((*gt != NULL) || (*lt != NULL))
  {
    if((*gt != NULL) && ((*lt == NULL) || is_full(runner, *gt)))
      repl = take_last_lt(runner, tree, gt);
    else repl = take_last_gt(runner, tree, lt);
    repl_node = NODE(runner, repl);

    update_gt_lt(runner, repl, repl_node, node_node);
  }

  if(repl_node != NULL) repl_node -> parent = parent;
  void ** repl_self = self_(runner, tree, node, node_node);
  *repl_self = repl;

  if(repl != NULL) rebalance_until(tree, runner, tree, repl_self);
  else if(parent != NULL) rebalance_until(tree, runner, tree,
    self_(runner, tree, parent, parent_node));
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
    update_eq(runner, parent, parent_node, node_node);
  }
  else if(node_node -> eq != NULL)
  {
    rm_first_eq(runner, tree, node, node_node);
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
