#ifndef __CMAP_LIFECYCLE_DEFINE_H__
#define __CMAP_LIFECYCLE_DEFINE_H__

#define CMAP_NATURE(lc) cmap_lifecycle_nature((CMAP_LIFECYCLE *)lc)

#define CMAP_INC_REFS(lc, proc_ctx) \
  cmap_lifecycle_inc_refs((CMAP_LIFECYCLE *)lc, proc_ctx)
#define CMAP_DEC_REFS(lc, proc_ctx) \
  cmap_lifecycle_dec_refs((CMAP_LIFECYCLE *)lc, proc_ctx)

#define CMAP_IS_GHOST(lc) cmap_lifecycle_is_ghost((CMAP_LIFECYCLE *)lc)

#endif
