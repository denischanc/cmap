#ifndef __CMAP_TREE_DEFINE_H__
#define __CMAP_TREE_DEFINE_H__

#define CMAP_TREE_EVALFN_NAME(prefix) prefix##_eval
#define CMAP_TREE_RUNNER_NAME(prefix) prefix##_runner

#define CMAP_TREE_RUNNER(prefix, gt_usable, lt_usable) \
static CMAP_TREE_RUNNER CMAP_TREE_RUNNER_NAME(prefix) = \
{ \
  cmap_tree_node, \
  CMAP_TREE_EVALFN_NAME(prefix), \
  gt_usable, lt_usable \
}

#define CMAP_TREE_FINDFN(prefix, tree, data) cmap_tree_public.find( \
  &CMAP_TREE_RUNNER_NAME(prefix), tree, (void *)data)

#define CMAP_TREE_ADDFN(prefix, tree, node, data) cmap_tree_public.add( \
  &CMAP_TREE_RUNNER_NAME(prefix), (void **)tree, node, (void *)data)
#define CMAP_TREE_RMFN(prefix, tree, node) cmap_tree_public.rm( \
  &CMAP_TREE_RUNNER_NAME(prefix), (void **)tree, node)

#define CMAP_TREE_APPLY(apply, before_fn, between_fn, after_fn) \
static CMAP_TREE_APPLY apply = \
{ \
  before_fn, \
  between_fn, \
  after_fn \
}

#define CMAP_TREE_APPLYFN(prefix, tree, apply_, gt_first, data) \
  cmap_tree_public.apply(&CMAP_TREE_RUNNER_NAME(prefix), tree, &apply_, \
    gt_first, (void *)data)

#define CMAP_TREE_CLEANFN(prefix, tree, clean_, data) \
  cmap_tree_public.clean(&CMAP_TREE_RUNNER_NAME(prefix), (void **)tree, \
    clean_, (void *)data)

#endif
