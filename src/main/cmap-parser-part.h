#ifndef __CMAP_PART_H__
#define __CMAP_PART_H__

#include "cmap-kv.h"

#define SPACE "  "

typedef struct
{
  void (*clean)();

  char ** (*functions)();
  char ** (*main)();

  char ** (*headers)();

  void (*new_ctx)();
  void (*push_instructions)();
  char ** (*instructions)();
  void (*add_instruction)(const char * instruction);
  void (*add_lf)();
  void (*prepend_instruction)(const char * instruction);
  char (*is_definitions)();
  char (*is_global_env)();
  void (*add_prefix)();
  CMAP_KV * (*name2map)();
  char * (*pop_instructions)();

  void (*return_)();
  char (*is_return)();

  void (*add_include)(const char * name);
  void (*add_relative_include)(const char * name);
  void (*add_include_lf)();
  char * (*includes)();
} CMAP_PARSER_PART_PUBLIC;

extern const CMAP_PARSER_PART_PUBLIC cmap_parser_part_public;

#endif
