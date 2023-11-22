#ifndef __CMAP_PARSER_UTIL_H__
#define __CMAP_PARSER_UTIL_H__

#include "cmap-parser-util-define.h"

typedef struct
{
  void (*function_c)(char * name, char is_static);
  void (*cmap_impl)();

  char * (*name)(char * name);
  char * (*path)(char * map, char * name);

  void (*set_local)(char * name, char * map);
  void (*set_global)(char * name, char * map);
  void (*set_path)(char * src, char * name, char * map);

  char * (*args_push)(char * list, char * map);

  char * (*arg_names)(char * name);
  char * (*arg_names_push)(char * list, char * name);

  char * (*args_map)(char * name, char * map);
  char * (*args_map_push)(char * list, char * name, char * map);

  char * (*map_args)(char * args, char * aisle);
  char * (*list_args)(char * args, char * aisle);
  char * (*string)(char * string, char * aisle);
  char * (*int_)(char * i, char * aisle);

  void (*return_)(char * map);

  char * (*process)(char * map, char * fn_name, char * args);
  char * (*process_fn)(char * fn, char * args);
  char * (*process_c)(char * fn_name, char need_ret);

  char * (*function)(char * args, char * aisle, char * fn_name);

  void (*c_impl)(char * impl);
  void (*c_impl_root)(char * impl);

  void (*if_)(char * cmp_name);
  void (*else_empty)();
  void (*else_if_start)();
  void (*else_if_stop)();
  void (*else_)();

  CMAP_PARSER_UTIL_CMP_LOOP(CMAP_PARSER_UTIL_CMP_DECL)
  char * (*cmp_unique)(char * map);

  char * (*new)(char * map, char * args, char * aisle);

  void (*set_sb_int)(char * map, char * i, char * map_src);
  void (*set_sb_string)(char * map, char * string, char * map_src);
  void (*set_sb_map)(char * map, char * map_i, char * map_src);
  char * (*sb_int)(char * map, char * i);
  char * (*sb_string)(char * map, char * string);
  char * (*sb_map)(char * map, char * map_i);

  char * (*aisle_names)(char * names);
  char * (*aisle_map)(char * map);

  char * (*names)(char * names, char * name);
} CMAP_PARSER_UTIL_PUBLIC;

extern const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public;

#endif
