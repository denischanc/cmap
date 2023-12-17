#ifndef __CMAP_TREE_DEFINE_H__
#define __CMAP_TREE_DEFINE_H__

#define CMAP_TREE_LOOP(macro) \
  macro(ge) \
  macro(lt) \
  macro(parent)

#define CMAP_TREE_WAY_FIELD(way) void * way;
#define CMAP_TREE_WAY_FN_PTR(way) \
  void ** (*way)(CMAP_TREE_RUNNER * this, void * node);
#define CMAP_TREE_WAY_FN(way) \
  void ** cmap_tree_##way(CMAP_TREE_RUNNER * this, void * node);
#define CMAP_TREE_WAY_SET(way) cmap_tree_##way,

#define CMAP_TREE_EVALFN_NAME(prefix) prefix##_eval
#define CMAP_TREE_RUNNER_NAME(prefix) prefix##_runner

#define CMAP_TREE_RUNNER(prefix, internal, lt_usable, gt_usable) \
static CMAP_TREE_RUNNER CMAP_TREE_RUNNER_NAME(prefix) = \
{ \
  internal, \
  CMAP_TREE_LOOP(CMAP_TREE_WAY_SET) \
  CMAP_TREE_EVALFN_NAME(prefix), \
  cmap_tree_usable_##lt_usable, \
  cmap_tree_usable_##gt_usable \
}

#define CMAP_TREE_FINDFN(prefix, tree, data) cmap_tree_public.find( \
  &CMAP_TREE_RUNNER_NAME(prefix), tree, (void *)data)

#define CMAP_TREE_ADDFN(prefix, tree, node, data) cmap_tree_public.add( \
  &CMAP_TREE_RUNNER_NAME(prefix), (void **)tree, node, (void *)data)
#define CMAP_TREE_RMFN(prefix, tree, node) cmap_tree_public.rm( \
  &CMAP_TREE_RUNNER_NAME(prefix), (void **)tree, node)

#define CMAP_TREE_APPLY(apply, internal, before_fn, between_fn, after_fn) \
static CMAP_TREE_APPLY apply = \
{ \
  internal, \
  before_fn, \
  between_fn, \
  after_fn \
}

#define CMAP_TREE_APPLYFN(prefix, tree, apply_, ge_first, data) \
  cmap_tree_public.apply(&CMAP_TREE_RUNNER_NAME(prefix), (void **)tree, \
    &apply_, ge_first, (void *)data)

#define CMAP_TREE_CLEANFN(prefix, tree, clean_, data) \
  cmap_tree_public.clean(&CMAP_TREE_RUNNER_NAME(prefix), (void **)tree, \
    clean_, (void *)data)

#endif
