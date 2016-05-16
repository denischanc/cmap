#ifndef __CMAP_TREE_DEFINE_H__
#define __CMAP_TREE_DEFINE_H__

#include "cmap-tree-internal.h"

#define CMAP_TREE_STRUCT void * ge_, * lt_, * parent_

#define CMAP_TREE_EVALFN_NAME(prefix) prefix##__eval
#define CMAP_TREE_RUNNER_NAME(prefix) prefix##_runner_

#define CMAP_TREE_RUNNER(cast, prefix, internal, _lt_usable, _gt_usable) \
CMAP_TREE_LOOP(CMAP_TREE_DECLARE_RUNNER_WAY, cast, prefix) \
 \
static CMAP_TREE_RUNNER CMAP_TREE_RUNNER_NAME(prefix) = \
{ \
  .internal_ = (void *)internal, \
  CMAP_TREE_LOOP(CMAP_TREE_INIT_RUNNER_WAY, cast, prefix) \
  .eval = CMAP_TREE_EVALFN_NAME(prefix), \
  .lt_usable = cmap_tree_usable_##_lt_usable, \
  .gt_usable = cmap_tree_usable_##_gt_usable \
};

#define CMAP_TREE_FINDFN(prefix, tree, data) \
  cmap_tree_find(&CMAP_TREE_RUNNER_NAME(prefix), tree, (void *)data)

#define CMAP_TREE_ADDFN(prefix, tree, node, data) \
  cmap_tree_add(&CMAP_TREE_RUNNER_NAME(prefix), (void **)tree, node, \
    (void *)data)
#define CMAP_TREE_RMFN(prefix, tree, node) \
  cmap_tree_rm(&CMAP_TREE_RUNNER_NAME(prefix), (void **)tree, node)

#define CMAP_TREE_APPLY_INIT(apply, internal, \
  before_fn, between_fn, after_fn) \
apply.internal_ = (void *)internal; \
apply.before = before_fn; \
apply.between = between_fn; \
apply.after = after_fn;

#define CMAP_TREE_APPLYFN(prefix, tree, apply, ge_first, data) \
  cmap_tree_apply(&CMAP_TREE_RUNNER_NAME(prefix), (void **)tree, &apply, \
    ge_first, (void *)data)

#endif
