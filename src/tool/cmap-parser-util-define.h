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
  char * (*name)(char * map_l, char * map_r);

#endif
