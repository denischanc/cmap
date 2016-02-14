#ifndef __CMAP_TREE_DEFINE_H__
#define __CMAP_TREE_DEFINE_H__

#include "cmap-tree-internal.h"

#define CMAP_TREE_STRUCT void * ge_, * lt_, * parent_

#define CMAP_TREE_RUNNER(cast, prefix, _lt_usable, _gt_usable) \
CMAP_TREE_LOOP(CMAP_TREE_DECLARE_RUNNER_WAY, cast, prefix) \
 \
static CMAP_TREE_RUNNER prefix##_runner_ = \
{ \
  CMAP_TREE_LOOP(CMAP_TREE_INIT_RUNNER_WAY, cast, prefix) \
  .eval = prefix##__eval, \
  .lt_usable = cmap_tree_usable_##_lt_usable, \
  .gt_usable = cmap_tree_usable_##_gt_usable \
};

#endif
