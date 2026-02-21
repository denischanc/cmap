#ifndef __CMAP_PART_DEFINE_H__
#define __CMAP_PART_DEFINE_H__

#define SPACE "  "

#define CMAP_PART_LOOP(macro) \
  macro(includes) \
  macro(main) \
  macro(headers)

#define CMAP_PART_DECL(part) char ** cmap_part_##part();

#define CMAP_PART_NATURE_DECL(NAME, name, val) \
  void cmap_part_nature_ctx_##name();

#endif
