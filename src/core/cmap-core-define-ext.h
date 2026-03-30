#ifndef __CMAP_CORE_DEFINE_EXT_H__
#define __CMAP_CORE_DEFINE_EXT_H__

#define CMAP_CORE_LOOP(macro) \
  macro(DOUBLE, double, 0, map, map) \
  macro(FN, fn, 1, fn, fn) \
  macro(INT, int, 2, map, map) \
  macro(LIST, list, 3, list, list) \
  macro(MAP, map, 4, map, map) \
  macro(PTR, ptr, 5, map, ptr) \
  macro(STRING, string, 6, map, string)

#define CMAP_CORE_NATURE_DECL(NAME, name, id, nested, delete) \
  extern const unsigned char CMAP_##NAME##_NATURE;

#endif
