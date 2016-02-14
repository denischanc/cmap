#ifndef __CMAP_TREE_INTERNAL_H__
#define __CMAP_TREE_INTERNAL_H__

#define CMAP_TREE_LOOP(macro, cast, prefix) \
  macro(cast, prefix, ge) \
  macro(cast, prefix, lt) \
  macro(cast, prefix, parent)

#define CMAP_TREE_DECLARE_RUNNER_WAY(cast, prefix, way) \
static void ** prefix##__##way(CMAP_TREE_RUNNER * this, void * node) \
{ \
  return &((cast *)node) -> way##_; \
}

#define CMAP_TREE_INIT_RUNNER_WAY(cast, prefix, way) \
  .way = prefix##__##way,

#endif
