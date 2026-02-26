#ifndef __CMAP_OP_HANDLER_DEFINE_H__
#define __CMAP_OP_HANDLER_DEFINE_H__

#define CMAP_OP_HANDLER_add_LOOP(macro) \
  macro(string, add) \
  macro(int, add) \
  macro(double, add)

#define CMAP_OP_HANDLER_sub_LOOP(macro) \
  macro(int, sub) \
  macro(double, sub)

#define CMAP_OP_HANDLER_mul_LOOP(macro) \
  macro(int, mul) \
  macro(double, mul)

#define CMAP_OP_HANDLER_div_LOOP(macro) \
  macro(int, div) \
  macro(double, div)

#define CMAP_OP_HANDLER_add_self_LOOP(macro) \
  macro(string, add_self) \
  macro(int, add_self) \
  macro(double, add_self)

#define CMAP_OP_HANDLER_sub_self_LOOP(macro) \
  macro(int, sub_self) \
  macro(double, sub_self)

#define CMAP_OP_HANDLER_mul_self_LOOP(macro) \
  macro(int, mul_self) \
  macro(double, mul_self)

#define CMAP_OP_HANDLER_div_self_LOOP(macro) \
  macro(int, div_self) \
  macro(double, div_self)

#define CMAP_OP_HANDLER_inc_LOOP(macro) \
  macro(int, inc)

#define CMAP_OP_HANDLER_dec_LOOP(macro) \
  macro(int, dec)

#define CMAP_OP_HANDLER_DECL(type, name) \
  extern const CMAP_OP_HANDLER cmap_op_handler_##type##_##name;

#define CMAP_OP_HANDLER_SELF_DECL(type, name) \
  extern const CMAP_OP_HANDLER_SELF cmap_op_handler_##type##_##name;

#define CMAP_OP_HANDLER_UNIQUE_DECL(type, name) \
  extern const CMAP_OP_HANDLER_UNIQUE cmap_op_handler_##type##_##name;

#define CMAP_OP_HANDLER_DEFINE(name, OP) \
  CMAP_OP_HANDLER_##name##_LOOP(CMAP_OP_HANDLER_DECL)

#define CMAP_OP_HANDLER_SELF_DEFINE(name, OP) \
  CMAP_OP_HANDLER_##name##_LOOP(CMAP_OP_HANDLER_SELF_DECL)

#define CMAP_OP_HANDLER_UNIQUE_DEFINE(name, OP) \
  CMAP_OP_HANDLER_##name##_LOOP(CMAP_OP_HANDLER_UNIQUE_DECL)

#endif
