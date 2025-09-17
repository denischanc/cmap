#ifndef __CMAP_PART_CTX_DEFINE_H__
#define __CMAP_PART_CTX_DEFINE_H__

#define CMAP_PART_CTX_NATURE_LOOP(macro) \
  macro(FN, fn, 0) \
  macro(PROC, proc, 1) \
  macro(CMP, cmp, 2) \
  macro(BLOCK, block, 3)

#define CMAP_PART_CTX_NATURE_DECL(NAME, name, val) void (*nature_##name)();

#endif
