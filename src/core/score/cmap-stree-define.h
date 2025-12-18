#ifndef __CMAP_STREE_DEFINE_H__
#define __CMAP_STREE_DEFINE_H__

#define CMAP_STREE_RUNNER_NAME(prefix) prefix##_stree_runner

#define CMAP_STREE_RUNNER(prefix, eval, log, gt_usable, lt_usable) \
static CMAP_STREE_RUNNER CMAP_STREE_RUNNER_NAME(prefix) = \
  {eval, log, gt_usable, lt_usable}

#define CMAP_STREE_FINDFN(prefix, stree, data) cmap_stree_public.find( \
  &CMAP_STREE_RUNNER_NAME(prefix), (CMAP_STREE_NODE *)stree, (void *)data)

#define CMAP_STREE_ADDFN(prefix, stree, node, data) cmap_stree_public.add( \
  &CMAP_STREE_RUNNER_NAME(prefix), (CMAP_STREE_NODE **)stree, \
  (CMAP_STREE_NODE *)node, (void *)data)
#define CMAP_STREE_RMFN(stree, node) cmap_stree_public.rm( \
  (CMAP_STREE_NODE **)stree, (CMAP_STREE_NODE *)node)

#define CMAP_STREE_APPLY(apply, before_fn, between_fn, after_fn) \
static CMAP_STREE_APPLY apply = {before_fn, between_fn, after_fn}

#define CMAP_STREE_APPLYFN(stree, apply_, gt_first, eq_apply, data) \
  cmap_stree_public.apply((CMAP_STREE_NODE *)stree, &apply_, \
    gt_first, eq_apply, (void *)data)

#define CMAP_STREE_QUICKAPPLYFN(stree, apply, data) \
  cmap_stree_public.quick_apply((CMAP_STREE_NODE *)stree, apply, (void *)data)

#define CMAP_STREE_LOGFN(prefix, lvl, stree) \
  cmap_stree_public.log(lvl, &CMAP_STREE_RUNNER_NAME(prefix), \
    (CMAP_STREE_NODE *)stree)

#endif
