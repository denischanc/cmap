#ifndef __CMAP_PARSER_PART_H__
#define __CMAP_PARSER_PART_H__

#include "cmap-parser-part-define.h"
#include "cmap-parser-params.h"

typedef struct
{
  CMAP_PARSER_PART_LOOP(CMAP_PARSER_PART_APPEND_DECL)

  void (*include_)(char * includes);
  CMAP_PARSER_PARAMS_RET (*function_c_to_part)(char ** part, char * name,
    char is_static);
  void (*function_c)(char * name, char is_static);
  void (*instructions_root)();

  char * (*function)(char * fn_name);

  void (*c_impl)(char * impl);
  void (*c_impl_root)(char * impl);
} CMAP_PARSER_PART_PUBLIC;

extern const CMAP_PARSER_PART_PUBLIC cmap_parser_part_public;

#endif
