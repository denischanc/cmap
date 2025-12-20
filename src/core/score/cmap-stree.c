
#include "cmap-stree.h"

#include <stdlib.h>
#include <sys/param.h>
#include <string.h>
#include "cmap.h"
#include "cmap-slist.h"
#include "cmap-mem.h"
#include "cmap-log.h"

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
  if((size == 0) || (size == 1)) return 0;

  size >>= 1;
  char depth = 0;
  while(size != 0)
  {
    size >>= 1;
    depth++;
  }
  return depth;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STREE_NODE * find(CMAP_STREE_RUNNER * runner,
  CMAP_STREE_NODE * stree, void * data)
{
  CMAP_STREE_NODE * result = NULL;

  while(stree != NULL)
  {
    int64_t v = runner -> eval(stree, data);
    if(v == 0)
    {
      CMAP_STREE_NODE * eq = stree -> eq;
      result = (eq != NULL) ? eq : stree;
      stree = NULL;
    }
    else if(v > 0)
    {
      if(runner -> gt_usable) result = stree;
      stree = stree -> lt;
    }
    else
    {
      if(runner -> lt_usable) result = stree;
      stree = stree -> gt;
    }
  }

  return result;
}

/*******************************************************************************
*******************************************************************************/

#define UPDATE_WAY_IMPL(way) \
static inline void update_##way(CMAP_STREE_NODE * node, \
  CMAP_STREE_NODE * old_node) \
{ \
  CMAP_STREE_NODE * way = old_node -> way; \
  node -> way = way; \
  if(way != NULL) way -> parent = node; \
}

UPDATE_WAY_IMPL(gt)
UPDATE_WAY_IMPL(eq)
UPDATE_WAY_IMPL(lt)

static inline void update_gt_lt(CMAP_STREE_NODE * node,
  CMAP_STREE_NODE * old_node)
{
  update_gt(node, old_node);
  update_lt(node, old_node);
}

/*******************************************************************************
*******************************************************************************/

static inline char update_depth(CMAP_STREE_NODE * node)
{
  int depth = 0;

  CMAP_STREE_NODE * gt = node -> gt;
  if(gt != NULL) depth = 1 + gt -> depth;

  CMAP_STREE_NODE * lt = node -> lt;
  if(lt != NULL)
  {
    int depth_lt = 1 + lt -> depth;
    if(depth_lt > depth) depth = depth_lt;
  }

  if(node -> depth != depth)
  {
    node -> depth = depth;
    return CMAP_T;
  }
  return CMAP_F;
}

/*******************************************************************************
*******************************************************************************/

static inline void update_depth_size_until(CMAP_STREE_NODE * stop, int v,
  CMAP_STREE_NODE * node)
{
  char update_depth_ = CMAP_T;
  while(node != NULL)
  {
    if(update_depth_) update_depth_ = update_depth(node);
    node -> size += v;

    if(node == stop) return;
    node = node -> parent;
  }
}

/*******************************************************************************
*******************************************************************************/

static inline void init_node_wo_eq(CMAP_STREE_NODE * node,
  CMAP_STREE_NODE * parent)
{
  node -> gt = NULL;
  node -> lt = NULL;
  node -> parent = parent;
  node -> depth = 0;
  node -> size = 1;
}

static inline void init_node(CMAP_STREE_NODE * node, CMAP_STREE_NODE * parent)
{
  init_node_wo_eq(node, parent);
  node -> eq = NULL;
}

/*******************************************************************************
*******************************************************************************/

static inline CMAP_STREE_NODE ** self_(CMAP_STREE_NODE ** stree,
  CMAP_STREE_NODE * node)
{
  CMAP_STREE_NODE * parent = node -> parent;
  if(parent == NULL) return stree;

  if(parent -> gt == node) return &parent -> gt;
  else return &parent -> lt;
}

/*******************************************************************************
*******************************************************************************/

#define GET_LAST_WAY_IMPL(way) \
static inline CMAP_STREE_NODE * get_last_##way(CMAP_STREE_NODE * node) \
{ \
  CMAP_STREE_NODE * way = node -> way; \
  while(way != NULL) \
  { \
    node = way; \
    way = way -> way; \
  } \
  return node; \
}

GET_LAST_WAY_IMPL(gt)
GET_LAST_WAY_IMPL(lt)

/*******************************************************************************
*******************************************************************************/

#define TAKE_LAST_WAY_IMPL(way, other) \
static inline CMAP_STREE_NODE * take_last_##way(CMAP_STREE_NODE * node) \
{ \
  CMAP_STREE_NODE * last = get_last_##way(node); \
  CMAP_STREE_NODE * parent = last -> parent; \
 \
  CMAP_STREE_NODE * other = last -> other; \
  if(other != NULL) other -> parent = parent; \
  *self_(NULL, last) = other; \
 \
  if(last != node) update_depth_size_until(node, -1, parent); \
 \
  return last; \
}

TAKE_LAST_WAY_IMPL(gt, lt)
TAKE_LAST_WAY_IMPL(lt, gt)

/*******************************************************************************
*******************************************************************************/

#define MV_WAY_TO_IMPL(way, other) \
static inline void mv_##way##_to_##other(CMAP_STREE_NODE ** self, \
  CMAP_STREE_NODE * node, CMAP_STREE_NODE * gt, CMAP_STREE_NODE * lt) \
{ \
  CMAP_STREE_NODE * repl = take_last_##other(way); \
  update_gt_lt(repl, node); \
  repl -> parent = node -> parent; \
  repl -> size = node -> size; \
  *self = repl; \
 \
  if(other == NULL) \
  { \
    repl -> other = node; \
    init_node_wo_eq(node, repl); \
  } \
  else \
  { \
    CMAP_STREE_NODE * last = get_last_##way(other); \
    last -> way = node; \
    init_node_wo_eq(node, last); \
    update_depth_size_until(other, 1, last); \
  } \
}

MV_WAY_TO_IMPL(gt, lt)
MV_WAY_TO_IMPL(lt, gt)

/*******************************************************************************
*******************************************************************************/

static void pack(CMAP_STREE_NODE ** self, CMAP_STREE_NODE * node, int depth);

static inline void pack_gt_lt(CMAP_STREE_NODE * node, CMAP_STREE_NODE * gt,
  CMAP_STREE_NODE * lt, int depth)
{
  if((gt != NULL) && (gt -> depth > depth)) pack(&node -> gt, gt, depth);

  if((lt != NULL) && (lt -> depth > depth)) pack(&node -> lt, lt, depth);

  node -> depth = 1 + depth;
}

#define MV_N_PACK_WAY_IMPL(way, other) \
static inline void mv_n_pack_##way(CMAP_STREE_NODE ** self, \
  CMAP_STREE_NODE * node, CMAP_STREE_NODE * gt, CMAP_STREE_NODE * lt, \
  int depth, int size) \
{ \
  int nb = way -> size - size; \
  for(int i = 0; i < nb; i++) \
  { \
    mv_##way##_to_##other(self, node, gt, lt); \
    node = *self; \
    gt = node -> gt; \
    lt = node -> lt; \
  } \
 \
  pack_gt_lt(node, gt, lt, depth); \
}

MV_N_PACK_WAY_IMPL(gt, lt)
MV_N_PACK_WAY_IMPL(lt, gt)

/*******************************************************************************
*******************************************************************************/

static void pack(CMAP_STREE_NODE ** self, CMAP_STREE_NODE * node, int depth)
{
  CMAP_STREE_NODE * gt = node -> gt, * lt = node -> lt;
  int size = size_max(--depth);
  if(gt == NULL) mv_n_pack_lt(self, node, NULL, lt, depth, size);
  else if(lt == NULL) mv_n_pack_gt(self, node, gt, NULL, depth, size);
  else
  {
    if(gt -> size > lt -> size) mv_n_pack_gt(self, node, gt, lt, depth, size);
    else mv_n_pack_lt(self, node, gt, lt, depth, size);
  }
}

/*******************************************************************************
*******************************************************************************/

static inline void add_eq(CMAP_STREE_NODE * node, CMAP_STREE_NODE * parent)
{
  node -> gt = NULL;
  update_eq(node, parent);
  node -> lt = NULL;
  node -> parent = parent;

  parent -> eq = node;
}

/*******************************************************************************
*******************************************************************************/

static void add(CMAP_STREE_RUNNER * runner, CMAP_STREE_NODE ** stree,
  CMAP_STREE_NODE * node, void * data)
{
  CMAP_STREE_NODE ** parent = NULL, ** cur = stree;
  while(*cur != NULL)
  {
    parent = cur;

    int64_t v = runner -> eval(*cur, data);
    if(v > 0) cur = &(*cur) -> lt;
    else if(v < 0) cur = &(*cur) -> gt;
    else
    {
      add_eq(node, *cur);
      return;
    }
  }

  init_node(node, (parent == NULL) ? NULL : *parent);
  *cur = node;

  if(parent != NULL)
  {
    update_depth_size_until(*stree, 1, *parent);

    CMAP_STREE_NODE * stree_ = *stree;
    int depth = depth_ok(stree_ -> size);
    if(stree_ -> depth > depth) pack(stree, stree_, depth);
  }
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_first_eq(CMAP_STREE_NODE ** stree, CMAP_STREE_NODE * node)
{
  CMAP_STREE_NODE * eq = node -> eq;
  update_gt_lt(eq, node);
  eq -> parent = node -> parent;
  eq -> depth = node -> depth;
  eq -> size = node -> size;

  *self_(stree, node) = eq;
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_not_eq_gt_lt(CMAP_STREE_NODE ** stree,
  CMAP_STREE_NODE * node, CMAP_STREE_NODE * gt, CMAP_STREE_NODE * lt,
  CMAP_STREE_NODE * parent)
{
  CMAP_STREE_NODE * repl;
  if(gt -> size > lt -> size) repl = take_last_lt(gt);
  else repl = take_last_gt(lt);

  update_gt_lt(repl, node);
  repl -> parent = parent;
  repl -> size = node -> size;

  if(parent == NULL) *stree = repl;
  else *self_(stree, node) = repl;

  update_depth_size_until(*stree, -1, repl);
}

/*******************************************************************************
*******************************************************************************/

static inline void rm_not_eq(CMAP_STREE_NODE ** stree, CMAP_STREE_NODE * node,
  CMAP_STREE_NODE * parent)
{
  CMAP_STREE_NODE * gt = node -> gt, * lt = node -> lt;
  if(gt == NULL)
  {
    if(lt != NULL) lt -> parent = parent;

    if(parent == NULL) *stree = lt;
    else
    {
      *self_(stree, node) = lt;
      update_depth_size_until(*stree, -1, parent);
    }
  }
  else
  {
    if(lt == NULL)
    {
      gt -> parent = parent;
      if(parent == NULL) *stree = gt;
      else
      {
        *self_(stree, node) = gt;
        update_depth_size_until(*stree, -1, parent);
      }
    }
    else rm_not_eq_gt_lt(stree, node, gt, lt, parent);
  }
}

/*******************************************************************************
*******************************************************************************/

static void rm(CMAP_STREE_NODE ** stree, CMAP_STREE_NODE * node)
{
  CMAP_STREE_NODE * parent = node -> parent;
  if((parent != NULL) && (parent -> eq == node))
    update_eq(parent, node);
  else if(node -> eq != NULL)
    rm_first_eq(stree, node);
  else rm_not_eq(stree, node, parent);
}

/*******************************************************************************
*******************************************************************************/

static void do_apply(CMAP_STREE_NODE * stree, CMAP_STREE_APPLY * apply,
  char gt_first, char eq_apply, void * data, char is_eq)
{
  CMAP_STREE_NODE * gt = stree -> gt, * lt = stree -> lt, * cur;

  if(apply -> before != NULL) apply -> before(stree, is_eq, data);

  cur = (gt_first ? gt : lt);
  if(cur != NULL) do_apply(cur, apply, gt_first, eq_apply, data, is_eq);

  cur = stree -> eq;
  if(eq_apply && (cur != NULL))
    do_apply(cur, apply, gt_first, eq_apply, data, CMAP_T);

  if(apply -> between != NULL) apply -> between(stree, is_eq, data);

  cur = (gt_first ? lt : gt);
  if(cur != NULL) do_apply(cur, apply, gt_first, eq_apply, data, is_eq);

  if(apply -> after != NULL) apply -> after(stree, is_eq, data);
}

static void apply(CMAP_STREE_NODE * stree, CMAP_STREE_APPLY * apply,
  char gt_first, char eq_apply, void * data)
{
  if(stree != NULL) do_apply(stree, apply, gt_first, eq_apply, data, CMAP_F);
}

/*******************************************************************************
*******************************************************************************/

static void do_quick_apply(CMAP_STREE_NODE * stree, CMAP_STREE_APPLY_FN apply,
  void * data, char is_eq)
{
  CMAP_STREE_NODE * gt = stree -> gt, * eq = stree -> eq, * lt = stree -> lt;
  if(gt != NULL) do_quick_apply(gt, apply, data, is_eq);
  if(eq != NULL) do_quick_apply(eq, apply, data, CMAP_T);
  if(lt != NULL) do_quick_apply(lt, apply, data, is_eq);

  apply(stree, is_eq, data);
}

static void quick_apply(CMAP_STREE_NODE * stree, CMAP_STREE_APPLY_FN apply,
  void * data)
{
  if(stree != NULL) do_quick_apply(stree, apply, data, CMAP_F);
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
  CMAP_MEM_VAR;
  char * ret = (char *)mem -> alloc(size);
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
  CMAP_MEM_VAR;
  CMAP_MEM_FREE(CMAP_CALL(prefix_before, pop), mem);
  CMAP_MEM_FREE(CMAP_CALL(prefix_between, pop), mem);
  CMAP_MEM_FREE(CMAP_CALL(prefix_after, pop), mem);
}

static void log_before_apply(CMAP_STREE_NODE * node, char is_eq, void * data)
{
  LOG_APPLY_DATA * data_ = data;
  CMAP_SLIST_CHAR_PTR * prefix_before = data_ -> prefix_before,
    * prefix_between = data_ -> prefix_between,
    * prefix_after = data_ -> prefix_after;
  char * left = *CMAP_CALL(prefix_before, last);
  log_push_cat(left, prefix_before, prefix_between, prefix_after);
}

static void log_between_apply(CMAP_STREE_NODE * node, char is_eq, void * data)
{
  LOG_APPLY_DATA * data_ = data;
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

static void log_after_apply(CMAP_STREE_NODE * node, char is_eq, void * data)
{
  LOG_APPLY_DATA * data_ = data;
  CMAP_SLIST_CHAR_PTR * prefix_before = data_ -> prefix_before,
    * prefix_between = data_ -> prefix_between,
    * prefix_after = data_ -> prefix_after;
  log_free(prefix_before, prefix_between, prefix_after);
}

CMAP_STREE_APPLY(log_apply, log_before_apply, log_between_apply,
  log_after_apply);

static void log_(char lvl, CMAP_STREE_RUNNER * runner, CMAP_STREE_NODE * stree)
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

  apply(stree, &log_apply, CMAP_T, CMAP_F, &data);

  CMAP_CALL(data.prefix_before, delete);
  CMAP_CALL(data.prefix_between, delete);
  CMAP_CALL(data.prefix_after, delete);
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
