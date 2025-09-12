#ifndef __CMAP_PARSER_UTIL_H__
#define __CMAP_PARSER_UTIL_H__

#include "cmap-parser-util-define.h"

typedef struct
{
  void (*append_instruction_args)(const char * txt, ...);
  void (*append_variable_args)(const char * txt, ...);
  void (*prepend_instruction_args)(const char * txt, ...);
  void (*append_instruction)(const char * txt);
  void (*append_lf)();

  void (*add_args)(char ** instruction, char * args);
  void (*append_instruction_args_args)(char * args, const char * txt, ...);

  const char * (*add_definitions)();
  const char * (*add_global_env)();

  void (*prepend_map_var)(const char * map_name);

  char * (*next_name)(const char * what);
} CMAP_PARSER_UTIL_PUBLIC;

extern const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public;

#endif
