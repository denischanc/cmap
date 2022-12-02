#ifndef __CMAP_PARSER_UTIL_DEFINE_H__
#define __CMAP_PARSER_UTIL_DEFINE_H__

#define CMAP_PARSER_UTIL_CMP_LOOP(macro) \
  macro(lt, <) \
  macro(gt, >) \
  macro(le, <=) \
  macro(ge, >=) \
  macro(equal, ==) \
  macro(diff, !=)

#define CMAP_PARSER_UTIL_CMP_DECL(name, op) \
  CMAP_MAP * (*name)(CMAP_MAP * map_l, CMAP_MAP * map_r);

#endif
