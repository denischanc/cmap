#ifndef __CMAP_PARSER_UTIL_H__
#define __CMAP_PARSER_UTIL_H__

#include "cmap-parser-string.h"

typedef struct
{
  CMAP_PARSER_STRING * (*include_string)();
  CMAP_PARSER_STRING * (*function_string)();
  CMAP_PARSER_STRING * (*main_string)();
  CMAP_PARSER_STRING * (*instructions_string)();

  void (*include)(char * name);

  void (*function_c)(char * name);

  char * (*name)(char * name);
  char * (*path)(char * map, char * name);

  void (*set_local)(char * name, char * map);
  void (*set_global)(char * name, char * map);
  void (*set_path)(char * src, char * name, char * map);

  char * (*args_push)(char * list, char * map);

  char * (*args_map)(char * name, char * map);
  char * (*args_map_push)(char * list, char * name, char * map);

  char * (*map_args)(char * args, char * aisle);

  char * (*list_args)(char * args, char * aisle);

  char * (*string)(char * string, char * aisle);

  char * (*int_)(char * i, char * aisle);
} CMAP_PARSER_UTIL_PUBLIC;

extern const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public;

#endif
