#ifndef __CMAP_PARSER_PART_H__
#define __CMAP_PARSER_PART_H__

#include "cmap-parser-part-define.h"

typedef struct
{
  void (*append_instructions)(const char * txt);

  void (*include_)(char * includes);
  void (*function_c)(char * name, char is_static);
  void (*instructions_root)();

  char * (*function)(char * fn_name);

  char * (*function_cmp)();

  void (*c_impl)(char * impl);
  void (*c_impl_root)(char * impl);

  char * (*for_iter)();
} CMAP_PARSER_PART_PUBLIC;

extern const CMAP_PARSER_PART_PUBLIC cmap_parser_part_public;

#endif
