#ifndef __CMAP_CORE_DEFINE_H__
#define __CMAP_CORE_DEFINE_H__

#define CMAP_CORE_NATURE_CHAR_DECL(NAME, name, id, nested, delete) \
  extern const char * CMAP_##NAME##_NATURE_CHAR;

#define CMAP_NATURE_CHAR(lc) cmap_core_nature_char((CMAP_LIFECYCLE *)lc)

#endif
