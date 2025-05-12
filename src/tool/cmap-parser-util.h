#ifndef __CMAP_PARSER_UTIL_H__
#define __CMAP_PARSER_UTIL_H__

#include "cmap-parser-util-define.h"

typedef struct
{
  void (*include_)(char * includes);
  void (*function_c)(char * name, char is_static);
  void (*instructions_root)();

  char * (*path)(char * map, char * name);
  char * (*name)(char * name);

  void (*set_local)(char * name, char * map);
  void (*set_path)(char * src, char * name, char * map);
  void (*set_global)(char * name, char * map);

  char * (*args)(char * map);
  char * (*args_push)(char * list, char * map);

  void (*arg_name)(char * name);

  char * (*args_map)(char * name, char * map);
  char * (*args_map_push)(char * list, char * name, char * map);

  char * (*map_args)(char * args);
  char * (*list_args)(char * args);
  char * (*string)(char * string);
  char * (*int_)(char * i);

  void (*return_)(char * map);

  char * (*process)(char * map, char * fn_name, char * args);
  char * (*process_fn)(char * fn, char * args);
  char * (*process_instruction)(char * txt, char need_ret);
  char * (*process_c)(char * fn_name, char need_ret);

  char * (*function)(char * fn_name);

  void (*c_impl)(char * impl);
  void (*c_impl_root)(char * impl);

  void (*if_)(char * cmp_call);
  void (*else_empty)();
  void (*else_if)();
  void (*else_)();

  CMAP_PARSER_UTIL_CMP_LOOP(CMAP_PARSER_UTIL_CMP_DECL)
  char * (*cmp_unique)(char * map);

  char * (*new)(char * map, char * args);

  void (*set_sb_int)(char * map, char * i, char * map_src);
  void (*set_sb_string)(char * map, char * string, char * map_src);
  void (*set_sb_map)(char * map, char * map_i, char * map_src);
  char * (*sb_int)(char * map, char * i);
  char * (*sb_string)(char * map, char * string);
  char * (*sb_map)(char * map, char * map_i);

  char * (*names)(char * names, char * name);

  char * (*for_helper)();
  void (*for_)(char * cmp_call, char * loop_call);
  void (*while_)(char * cmp_call);

  char * (*or)(char * cmp_call_l, char * cmp_call_r);
  char * (*and)(char * cmp_call_l, char * cmp_call_r);
} CMAP_PARSER_UTIL_PUBLIC;

extern const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public;

#endif
