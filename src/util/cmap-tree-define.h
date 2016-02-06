#ifndef __CMAP_TREE_DEFINE_H__
#define __CMAP_TREE_DEFINE_H__

#define CMAP_TREE_LOOP(macro, runner, cast, prefix) \
  macro(runner, cast, prefix, ge) \
  macro(runner, cast, prefix, lt) \
  macro(runner, cast, prefix, parent)

#define CMAP_TREE_DECLARE_RUNNER_WAY(runner, cast, prefix, way) \
static void ** prefix##__##way(CMAP_TREE_RUNNER * this, void * node) \
{ \
  return (void **)&((cast *)node) -> way##_; \
}

#define CMAP_TREE_INIT_RUNNER_WAY(runner, cast, prefix, way) \
  runner.way = prefix##__##way;

#define CMAP_TREE_DECLARE_RUNNER(cast, prefix) \
  CMAP_TREE_LOOP(CMAP_TREE_DECLARE_RUNNER_WAY, runner, cast, prefix)

#define CMAP_TREE_INIT_RUNNER(runner, prefix, _lt_usable, _gt_usable) \
  CMAP_TREE_LOOP(CMAP_TREE_INIT_RUNNER_WAY, runner, cast, prefix) \
  runner.lt_usable = cmap_tree_usable_##_lt_usable; \
  runner.gt_usable = cmap_tree_usable_##_gt_usable;

#endif
