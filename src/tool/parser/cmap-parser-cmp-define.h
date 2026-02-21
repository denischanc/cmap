#ifndef __CMAP_PARSER_CMP_DEFINE_H__
#define __CMAP_PARSER_CMP_DEFINE_H__

#define CMAP_PARSER_CMP_LOOP(macro) \
  macro(lt, <) \
  macro(gt, >) \
  macro(le, <=) \
  macro(ge, >=) \
  macro(equal, ==) \
  macro(diff, !=)

#define CMAP_PARSER_CMP_DECL(name, op) \
  char * cmap_parser_cmp_##name(char * map_l, char * map_r);

#endif
