#ifndef __CMAP_PART_CTX_DEFINE_H__
#define __CMAP_PART_CTX_DEFINE_H__

#define CMAP_PART_CTX_NATURE_LOOP(macro) \
  macro(FN, fn) \
  macro(PARAMS, params) \
  macro(PROC, proc) \
  macro(CMP, cmp)

#define CMAP_PART_CTX_NATURE_DECL(NAME, name) void (*nature_##name)();

#endif
