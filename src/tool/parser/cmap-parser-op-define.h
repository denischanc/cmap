#ifndef __CMAP_PARSER_OP_DEFINE_H__
#define __CMAP_PARSER_OP_DEFINE_H__

#define CMAP_PARSER_OP_LOOP(macro) \
  macro(add) \
  macro(sub) \
  macro(mul) \
  macro(div)

#define CMAP_PARSER_OP_SELF_LOOP(macro) \
  macro(add_self) \
  macro(sub_self) \
  macro(mul_self) \
  macro(div_self)

#define CMAP_PARSER_OP_UNIQUE_LOOP(macro) \
  macro(inc) \
  macro(dec)

#define CMAP_PARSER_OP_DECL(name) \
  char * cmap_parser_op_##name(char * map_l, char * map_r);

#define CMAP_PARSER_OP_SELF_DECL(name) \
  void cmap_parser_op_##name(char * map_dst, char * map_src);

#define CMAP_PARSER_OP_UNIQUE_DECL(name) \
  void cmap_parser_op_##name(char * map);

#endif
