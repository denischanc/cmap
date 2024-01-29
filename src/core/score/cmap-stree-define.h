#ifndef __CMAP_STREE_DEFINE_H__
#define __CMAP_STREE_DEFINE_H__

#define CMAP_STREE_EVALFN_NAME(prefix) prefix##_stree_eval
#define CMAP_STREE_RUNNER_NAME(prefix) prefix##_stree_runner

#define CMAP_STREE_RUNNER(prefix, log, gt_usable, lt_usable) \
static CMAP_STREE_RUNNER CMAP_STREE_RUNNER_NAME(prefix) = \
{ \
  cmap_stree_node, \
  CMAP_STREE_EVALFN_NAME(prefix), \
  log, \
  gt_usable, lt_usable \
}

#define CMAP_STREE_FINDFN(prefix, stree, data) cmap_stree_public.find( \
  &CMAP_STREE_RUNNER_NAME(prefix), stree, (void *)data)

#define CMAP_STREE_ADDFN(prefix, stree, node, data) cmap_stree_public.add( \
  &CMAP_STREE_RUNNER_NAME(prefix), (void **)stree, node, (void *)data)
#define CMAP_STREE_RMFN(prefix, stree, node) cmap_stree_public.rm( \
  &CMAP_STREE_RUNNER_NAME(prefix), (void **)stree, node)

#define CMAP_STREE_APPLY(apply, before_fn, between_fn, after_fn) \
static CMAP_STREE_APPLY apply = \
{ \
  before_fn, \
  between_fn, \
  after_fn \
}

#define CMAP_STREE_APPLYFN(prefix, stree, apply_, gt_first, eq_apply, data) \
  cmap_stree_public.apply(&CMAP_STREE_RUNNER_NAME(prefix), stree, &apply_, \
    gt_first, eq_apply, (void *)data)

#define CMAP_STREE_QUICKAPPLYFN(prefix, stree, apply, data) \
  cmap_stree_public.quick_apply(&CMAP_STREE_RUNNER_NAME(prefix), stree, \
    apply, (void *)data)

#define CMAP_STREE_LOGFN(prefix, lvl, stree) \
  cmap_stree_public.log(lvl, &CMAP_STREE_RUNNER_NAME(prefix), stree)

#endif
