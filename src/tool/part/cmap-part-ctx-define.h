#ifndef __CMAP_PART_CTX_DEFINE_H__
#define __CMAP_PART_CTX_DEFINE_H__

#define CMAP_PART_CTX_NATURE_LOOP(macro) \
  macro(FN, fn, 0) \
  macro(ITER, iter, 1) \
  macro(CMP, cmp, 2) \
  macro(BLOCK, block, 3) \
  macro(LOOP, loop, 4)

#define CMAP_PART_CTX_NATURE_DECL(NAME, name, val) void (*nature_##name)();

#define CMAP_PART_CTX_FEATURE_LOOP(macro) \
  macro(ctx_cmap, (1 << 0)) \
  macro(ctx_fn_c, (1 << 1)) \
  macro(ctx_c, (1 << 2)) \
  macro(fwd_name, (1 << 3))

#define CMAP_PART_CTX_FEATURE_DECL(name, val) \
  char (*is_feature_##name)(CMAP_PART_CTX * ctx);

#endif
