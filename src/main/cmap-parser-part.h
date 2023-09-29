#ifndef __CMAP_PART_H__
#define __CMAP_PART_H__

#define SPACE "  "

typedef struct
{
  char ** (*c_impl)();
  char ** (*functions)();
  char ** (*main)();

  char ** (*headers)();

  void (*new_ctx)();
  void (*push_instructions)();
  char ** (*instructions)();
  void (*add_instruction)(const char * instruction);
  void (*add_lf)();
  void (*add_definitions)(const char * instruction);
  void (*add_global_env)(const char * instruction);
  void (*add_prefix)();
  char * (*pop_instructions)();

  void (*return_)();
  char (*is_return)();

  void (*add_include)(const char * name);
  void (*add_include_lf)();
  char * (*includes)();
} CMAP_PARSER_PART_PUBLIC;

extern const CMAP_PARSER_PART_PUBLIC cmap_parser_part_public;

#endif
