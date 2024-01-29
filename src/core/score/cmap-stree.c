
#include "cmap-stree.h"

#include <stdlib.h>
#include <sys/param.h>
#include <string.h>
#include "cmap.h"
#include "cmap-slist.h"
#include "cmap-kernel.h"
#include "cmap-log.h"

/*******************************************************************************
*******************************************************************************/

#define NODE(runner, node_) runner -> node(node_)

/*******************************************************************************
*******************************************************************************/

#define DEPTH_SIZES_MAX 31

static int depth_sizes[DEPTH_SIZES_MAX];
static char depth_sizes_ok = CMAP_F;

/*******************************************************************************
*******************************************************************************/

static inline void init_depth_sizes()
{
  int size = 0;
  for(int i = 0; i < DEPTH_SIZES_MAX; i++)
  {
    size <<= 1;
    size++;
    depth_sizes[i] = size;
  }

  depth_sizes_ok = CMAP_T;
}

static inline int size_max(char depth)
{
  if(!depth_sizes_ok) init_depth_sizes();
  return depth_sizes[(int)depth];
}

/*******************************************************************************
*******************************************************************************/

static inline char depth_ok(int size)
{
  if(size == 0) return 0;
  else
  {
    char depth = -1;
    while(size != 0)
    {
      size >>= 1;
      depth++;
    }
    return depth;
  }
}

/*******************************************************************************
*******************************************************************************/

static void * find(CMAP_STREE_RUNNER * runner, void * stree, void * data)
{
  void * result = NULL;

  while(stree != NULL)
  {
    int v = runner -> eval(stree, data);
    if(v == 0)
    {
      void * eq = NODE(runner, stree) -> eq;
      result = (eq != NULL) ? eq : stree;
      stree = NULL;
    }
    else if(v > 0)
    {
      if(runner -> gt_usable) result = stree;
      stree = NODE(runner, stree) -> lt;
    }
    else
    {
      if(runner -> lt_usable) result = stree;
      stree = NODE(runner, stree) -> gt;
    }
  }

  return result;
}

/*******************************************************************************
*******************************************************************************/

#define UPDATE_WAY_IMPL(way) \
static inline void update_##way(CMAP_STREE_RUNNER * runner, void * node, \
  CMAP_STREE_NODE * node_node, CMAP_STREE_NODE * old_node_node) \
{ \
  void * way = old_node_node -> way; \
  node_node -> way = way; \
  if(way != NULL) NODE(runner, way) -> parent = node; \
}

UPDATE_WAY_IMPL(gt)
UPDATE_WAY_IMPL(eq)
UPDATE_WAY_IMPL(lt)

static inline void update_gt_lt(CMAP_STREE_RUNNER * runner, void * node,
  CMAP_STREE_NODE * node_node, CMAP_STREE_NODE * old_node_node)
{
  update_gt(runner, node, node_node, old_node_node);
  update_lt(runner, node, node_node, old_node_node);
}

/*******************************************************************************
*******************************************************************************/

static inline char update_depth(CMAP_STREE_RUNNER * runner,
  CMAP_STREE_NODE * node_node)
{
  int depth = 0;

  void * gt = node_node -> gt;
  if(gt != NULL)
  {
    CMAP_STREE_NODE * gt_node = NODE(runner, gt);
    depth = 1 + gt_node -> depth;
  }

  void * lt = node_node -> lt;
  if(lt != NULL)
  {
    CMAP_STREE_NODE * lt_node = NODE(runner, lt);
    int depth_lt = 1 + lt_node -> depth;
    if(depth_lt > depth) depth = depth_lt;
  }

  if(node_node -> depth != depth)
  {
    node_node -> depth = depth;
    return CMAP_T;
  }
  return CMAP_F;
}

/*******************************************************************************
*******************************************************************************/

static inline void update_depth_size_until(void * stop, int v,
  CMAP_STREE_RUNNER * runner, void * node)
{
  char update_depth_ = CMAP_T;
  while(node != NULL)
  {
    CMAP_STREE_NODE * node_node = NODE(runner, node);

    if(update_depth_) update_depth_ = update_depth(runner, node_node);
    node_node -> size += v;

    if(node == stop) return;
    node = node_node -> parent;
  }
}

/*******************************************************************************
*******************************************************************************/

static inline void init_node_wo_eq(CMAP_STREE_NODE * node_node, void * parent)
{
  node_node -> gt = NULL;
  node_node -> lt = NULL;
  node_node -> parent = parent;
  node_node -> depth = 0;
  node_node -> size = 1;
}

static inline void init_node(CMAP_STREE_NODE * node_node, void * parent)
{
  init_node_wo_eq(node_node, parent);
  node_node -> eq = NULL;
}

/*******************************************************************************
*******************************************************************************/

static inline void ** self_(CMAP_STREE_RUNNER * runner, void ** stree,
  void * node, CMAP_STREE_NODE * node_node)
{
  void * parent = node_node -> parent;
  if(parent == NULL) return stree;

  CMAP_STREE_NODE * parent_node = NODE(runner, parent);
  if(parent_node -> gt == node) return &parent_node -> gt;
  else return &parent_node -> lt;
}

/*******************************************************************************
*******************************************************************************/

#define GET_LAST_WAY_IMPL(way) \
static inline void * get_last_##way(CMAP_STREE_RUNNER * runner, void * node, \
  CMAP_STREE_NODE * node_node) \
{ \
  void * way = node_node -> way; \
  while(way != NULL) \
  { \
    node = way; \
    way = NODE(runner, way) -> way; \
  } \
  return node; \
}

GET_LAST_WAY_IMPL(gt)
GET_LAST_WAY_IMPL(lt)

/*******************************************************************************
*******************************************************************************/

#define TAKE_LAST_WAY_IMPL(way, other) \
static inline void * take_last_##way(CMAP_STREE_RUNNER * runner, \
  void ** stree, void * node, CMAP_STREE_NODE * node_node) \
{ \
  void * last = get_last_##way(runner, node, node_node); \
  CMAP_STREE_NODE * last_node = NODE(runner, last); \
  void * parent = last_node -> parent; \
 \
  void * other = last_node -> other; \
  if(other != NULL) NODE(runner, other) -> parent = parent; \
  *self_(runner, stree, last, last_node) = other; \
 \
  if(last != node) update_depth_size_until(node, -1, runner, parent); \
 \
  return last; \
}

TAKE_LAST_WAY_IMPL(gt, lt)
TAKE_LAST_WAY_IMPL(lt, gt)

/*******************************************************************************
*******************************************************************************/

#define MV_WAY_TO_IMPL(way, other) \
static inline void mv_##way##_to_##other(CMAP_STREE_RUNNER * runner, \
  void ** stree, void ** self, void * node, CMAP_STREE_NODE * node_node, \
  void * gt, CMAP_STREE_NODE * gt_node, void * lt, CMAP_STREE_NODE * lt_node) \
{ \
  void * repl = take_last_##other(runner, stree, way, way##_node); \
  CMAP_STREE_NODE * repl_node = NODE(runner, repl); \
  update_gt_lt(runner, repl, repl_node, node_node); \
  repl_node -> parent = node_node -> parent; \
  repl_node -> size = node_node -> size; \
  *self = repl; \
 \
  if(other == NULL) \
  { \
    repl_node -> other = node; \
    init_node_wo_eq(node_node, repl); \
  } \
  else \
  { \
    void * last = get_last_##way(runner, other, other##_node); \
    CMAP_STREE_NODE * last_node = NODE(runner, last); \
    last_node -> way = node; \
    init_node_wo_eq(node_node, last); \
    update_depth_size_until(other, 1, runner, last); \
  } \
}

MV_WAY_TO_IMPL(gt, lt)
MV_WAY_TO_IMPL(lt, gt)

/*******************************************************************************
*******************************************************************************/

static void pack(CMAP_STREE_RUNNER * runner, void ** stree, void ** node,
  CMAP_STREE_NODE * node_node, int depth);

static inline void pack_gt_lt(CMAP_STREE_RUNNER * runner, void ** stree,
  CMAP_STREE_NODE * node_node, CMAP_STREE_NODE * gt_node,
  CMAP_STREE_NODE * lt_node, int depth)
{
  if((gt_node != NULL) && (gt_node -> depth > depth))
    pack(runner, stree, &node_node -> gt, gt_node, depth);

  if((lt_node != NULL) && (lt_node -> depth > depth))
    pack(runner, stree, &node_node -> lt, lt_node, depth);

  node_node -> depth = 1 + depth;
}

#define MV_N_PACK_WAY_IMPL(way, other) \
static inline void mv_n_pack_##way(CMAP_STREE_RUNNER * runner, void ** stree, \
  void ** node, CMAP_STREE_NODE * node_node, void * gt, \
  CMAP_STREE_NODE * gt_node, void * lt, CMAP_STREE_NODE * lt_node, int depth, \
  int size) \
{ \
  int nb = way##_node -> size - size; \
  for(int i = 0; i < nb; i++) \
  { \
    mv_##way##_to_##other(runner, stree, node, *node, node_node, gt, gt_node, \
      lt, lt_node); \
    node_node = NODE(runner, *node); \
    gt = node_node -> gt; gt_node = NODE(runner, gt); \
    lt = node_node -> lt; lt_node = NODE(runner, lt); \
  } \
 \
  pack_gt_lt(runner, stree, node_node, gt_node, lt_node, depth); \
}

MV_N_PACK_WAY_IMPL(gt, lt)
MV_N_PACK_WAY_IMPL(lt, gt)

/*******************************************************************************
*******************************************************************************/

static void pack(CMAP_STREE_RUNNER * runner, void ** stree, void ** node,
  CMAP_STREE_NODE * node_node, int depth)
{
  void * gt = node_node -> gt, * lt = node_node -> lt;
  int size = size_max(--depth);
  if(gt == NULL) mv_n_pack_lt(runner, stree, node, node_node, NULL, NULL, lt,
    NODE(runner, lt), depth, size);
  else if(lt == NULL) mv_n_pack_gt(runner, stree, node, node_node, gt,
    NODE(runner, gt), NULL, NULL, depth, size);
  else
  {
    CMAP_STREE_NODE * gt_node = NODE(runner, gt), * lt_node = NODE(runner, lt);
    if(gt_node -> size > lt_node -> size) mv_n_pack_gt(runner, stree, node,
      node_node, gt, gt_node, lt, lt_node, depth, size);
    else mv_n_pack_lt(runner, stree, node, node_node, gt, gt_node, lt, lt_node,
      depth, size);
  }
}

/*******************************************************************************
*******************************************************************************/

static inline void add_eq(CMAP_STREE_RUNNER * runner, void * node,
  void * parent)
{
  CMAP_STREE_NODE * node_node = NODE(runner, node),
    * parent_node = NODE(runner, parent);

  node_node -> gt = NULL;
  update_eq(runner, node, node_node, parent_node);
  node_node -> lt = NULL;
  node_node -> parent = parent;

  parent_node -> eq = node;
}

/*******************************************************************************
*******************************************************************************/

static void add(CMAP_STREE_RUNNER * runner, void ** stree, void * node,
  void * data)
{
  void ** parent = NULL, ** cur = stree;
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

  if(parent != NULL)
  {
    update_depth_size_until(*stree, 1, runner, *parent);

    CMAP_STREE_NODE * stree_node = NODE(runner, *stree);
    int depth = depth_ok(stree_node -> size);
    if(stree_node -> depth > depth)
      pack(runner, stree, stree, stree_node, depth);
  }
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_first_eq(CMAP_STREE_RUNNER * runner, void ** stree,
  void * node, CMAP_STREE_NODE * node_node)
{
  void * eq = node_node -> eq;
  CMAP_STREE_NODE * eq_node = NODE(runner, eq);
  update_gt_lt(runner, eq, eq_node, node_node);
  eq_node -> parent = node_node -> parent;
  eq_node -> depth = node_node -> depth;
  eq_node -> size = node_node -> size;

  *self_(runner, stree, node, node_node) = eq;
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_not_eq_gt_lt(CMAP_STREE_RUNNER * runner, void ** stree,
  void * node, CMAP_STREE_NODE * node_node, void * gt, void * lt,
  void * parent)
{
  CMAP_STREE_NODE * gt_node = NODE(runner, gt), * lt_node = NODE(runner, lt);
  void * repl;
  if(gt_node -> size > lt_node -> size)
    repl = take_last_gt(runner, stree, gt, gt_node);
  else repl = take_last_lt(runner, stree, lt, lt_node);
  CMAP_STREE_NODE * repl_node = NODE(runner, repl);

  update_gt_lt(runner, repl, repl_node, node_node);
  repl_node -> parent = parent;
  repl_node -> size = node_node -> size;

  if(parent == NULL) *stree = repl;
  else *self_(runner, stree, node, node_node) = repl;

  update_depth_size_until(*stree, -1, runner, repl);
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_not_eq(CMAP_STREE_RUNNER * runner, void ** stree,
  void * node, CMAP_STREE_NODE * node_node, void * parent)
{
  void * gt = node_node -> gt, * lt = node_node -> lt;
  if(gt == NULL)
  {
    if(lt != NULL)
    {
      CMAP_STREE_NODE * lt_node = NODE(runner, lt);
      lt_node -> parent = parent;
    }
    if(parent == NULL) *stree = lt;
    else
    {
      *self_(runner, stree, node, node_node) = lt;
      update_depth_size_until(*stree, -1, runner, parent);
    }
  }
  else
  {
    if(lt == NULL)
    {
      CMAP_STREE_NODE * gt_node = NODE(runner, gt);
      gt_node -> parent = parent;
      if(parent == NULL) *stree = gt;
      else
      {
        *self_(runner, stree, node, node_node) = gt;
        update_depth_size_until(*stree, -1, runner, parent);
      }
    }
    else rm_not_eq_gt_lt(runner, stree, node, node_node, gt, lt, parent);
  }
}

/*******************************************************************************
*******************************************************************************/

static void rm(CMAP_STREE_RUNNER * runner, void ** stree, void * node)
{
  CMAP_STREE_NODE * node_node = NODE(runner, node), * parent_node = NULL;
  void * parent = node_node -> parent;

  if(parent != NULL) parent_node = NODE(runner, parent);

  if((parent_node != NULL) && (parent_node -> eq == node))
    update_eq(runner, parent, parent_node, node_node);
  else if(node_node -> eq != NULL)
    rm_first_eq(runner, stree, node, node_node);
  else rm_not_eq(runner, stree, node, node_node, parent);
}

/*******************************************************************************
*******************************************************************************/

static void do_apply(CMAP_STREE_RUNNER * runner, void * stree,
  CMAP_STREE_APPLY * apply, char gt_first, char eq_apply, void * data,
  char is_eq)
{
  CMAP_STREE_NODE * stree_node = NODE(runner, stree);
  void * gt = stree_node -> gt, * lt = stree_node -> lt, * cur;

  if(apply -> before != NULL) apply -> before(stree, is_eq, data);

  cur = (gt_first ? gt : lt);
  if(cur != NULL)
    do_apply(runner, cur, apply, gt_first, eq_apply, data, is_eq);

  cur = stree_node -> eq;
  if(eq_apply && (cur != NULL))
    do_apply(runner, cur, apply, gt_first, eq_apply, data, CMAP_T);

  if(apply -> between != NULL) apply -> between(stree, is_eq, data);

  cur = (gt_first ? lt : gt);
  if(cur != NULL)
    do_apply(runner, cur, apply, gt_first, eq_apply, data, is_eq);

  if(apply -> after != NULL) apply -> after(stree, is_eq, data);
}

static void apply(CMAP_STREE_RUNNER * runner, void * stree,
  CMAP_STREE_APPLY * apply, char gt_first, char eq_apply, void * data)
{
  if(stree != NULL)
    do_apply(runner, stree, apply, gt_first, eq_apply, data, CMAP_F);
}

/*******************************************************************************
*******************************************************************************/

static void do_quick_apply(CMAP_STREE_RUNNER * runner, void * stree,
  CMAP_STREE_APPLY_FN apply, void * data, char is_eq)
{
  CMAP_STREE_NODE * stree_node = NODE(runner, stree);
  void * gt = stree_node -> gt, * eq = stree_node -> eq,
    * lt = stree_node -> lt;
  if(gt != NULL) do_quick_apply(runner, gt, apply, data, is_eq);
  if(eq != NULL) do_quick_apply(runner, eq, apply, data, CMAP_T);
  if(lt != NULL) do_quick_apply(runner, lt, apply, data, is_eq);

  apply(stree, is_eq, data);
}

static void quick_apply(CMAP_STREE_RUNNER * runner, void * stree,
  CMAP_STREE_APPLY_FN apply, void * data)
{
  if(stree != NULL) do_quick_apply(runner, stree, apply, data, CMAP_F);
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_SLIST_CHAR_PTR * prefix_before, * prefix_between, * prefix_after;
  char log_lvl;
  CMAP_STREE_RUNNER * runner;
} LOG_APPLY_DATA;

static char * log_cat(char * left, const char * right)
{
  int size = strlen(left) + strlen(right) + 1;
  size *= sizeof(char);
  char * ret = (char *)CMAP_KERNEL_MEM -> alloc(size);
  *ret = 0;
  strcat(ret, left);
  strcat(ret, right);
  return ret;
}

static void log_push_cat(char * left, CMAP_SLIST_CHAR_PTR * prefix_before,
  CMAP_SLIST_CHAR_PTR * prefix_between, CMAP_SLIST_CHAR_PTR * prefix_after)
{
  CMAP_CALL_ARGS(prefix_before, push, log_cat(left, "   "));
  CMAP_CALL_ARGS(prefix_between, push, log_cat(left, "  +"));
  CMAP_CALL_ARGS(prefix_after, push, log_cat(left, "  |"));
}

static void log_free(CMAP_SLIST_CHAR_PTR * prefix_before,
  CMAP_SLIST_CHAR_PTR * prefix_between, CMAP_SLIST_CHAR_PTR * prefix_after)
{
  CMAP_MEM * mem = CMAP_KERNEL_MEM;
  CMAP_MEM_FREE(CMAP_CALL(prefix_before, pop), mem);
  CMAP_MEM_FREE(CMAP_CALL(prefix_between, pop), mem);
  CMAP_MEM_FREE(CMAP_CALL(prefix_after, pop), mem);
}

static void log_before_apply(void * node, char is_eq, void * data)
{
  LOG_APPLY_DATA * data_ = (LOG_APPLY_DATA *)data;
  CMAP_SLIST_CHAR_PTR * prefix_before = data_ -> prefix_before,
    * prefix_between = data_ -> prefix_between,
    * prefix_after = data_ -> prefix_after;
  char * left = *CMAP_CALL(prefix_before, last);
  log_push_cat(left, prefix_before, prefix_between, prefix_after);
}

static void log_between_apply(void * node, char is_eq, void * data)
{
  LOG_APPLY_DATA * data_ = (LOG_APPLY_DATA *)data;
  CMAP_SLIST_CHAR_PTR * prefix_before = data_ -> prefix_before,
    * prefix_between = data_ -> prefix_between,
    * prefix_after = data_ -> prefix_after;

  log_free(prefix_before, prefix_between, prefix_after);

  char * left = *CMAP_CALL(prefix_between, last);
  const char * right = data_ -> runner -> log(node);
  cmap_log_public.log(data_ -> log_lvl, "%s%s", left, right);

  left = *CMAP_CALL(prefix_after, last);
  log_push_cat(left, prefix_after, prefix_between, prefix_before);
}

static void log_after_apply(void * node, char is_eq, void * data)
{
  LOG_APPLY_DATA * data_ = (LOG_APPLY_DATA *)data;
  CMAP_SLIST_CHAR_PTR * prefix_before = data_ -> prefix_before,
    * prefix_between = data_ -> prefix_between,
    * prefix_after = data_ -> prefix_after;
  log_free(prefix_before, prefix_between, prefix_after);
}

CMAP_STREE_APPLY(log_apply, log_before_apply, log_between_apply,
  log_after_apply);

static void log_(char lvl, CMAP_STREE_RUNNER * runner, void * stree)
{
  LOG_APPLY_DATA data;
  data.prefix_before = cmap_slist_char_ptr_public.create(0);
  data.prefix_between = cmap_slist_char_ptr_public.create(0);
  data.prefix_after = cmap_slist_char_ptr_public.create(0);
  data.log_lvl = lvl;
  data.runner = runner;

  CMAP_CALL_ARGS(data.prefix_before, push, (char *)" ");
  CMAP_CALL_ARGS(data.prefix_between, push, (char *)"+");
  CMAP_CALL_ARGS(data.prefix_after, push, (char *)" ");

  apply(runner, stree, &log_apply, CMAP_T, CMAP_F, &data);

  CMAP_CALL(data.prefix_before, delete);
  CMAP_CALL(data.prefix_between, delete);
  CMAP_CALL(data.prefix_after, delete);
}

/*******************************************************************************
*******************************************************************************/

CMAP_STREE_NODE * cmap_stree_node(void * node)
{
  return (CMAP_STREE_NODE *)node;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_STREE_PUBLIC cmap_stree_public =
{
  find,
  add, rm,
  apply, quick_apply,
  log_
};
