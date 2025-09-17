#ifndef __CMAP_PART_H__
#define __CMAP_PART_H__

#include "cmap-part-define.h"
#include "cmap-part-var.h"
#include "cmap-strings.h"
#include "cmap-part-keys.h"

#define SPACE "  "

typedef struct
{
  void (*clean)();

  CMAP_PART_LOOP(CMAP_PART_DECL)

  void (*push_instructions)();
  char ** (*instructions)();
  void (*add_instruction)(const char * instruction);
  void (*add_variable)(const char * variable);
  void (*add_lf)();
  void (*prepend_instruction)(const char * instruction);
  char (*is_definitions)();
  char (*is_global_env)();
  char * (*pop_instructions)();

  void (*return_)();
  char (*is_return)();
  char (*return_fn)();

  void (*add_include)(const char * name, char is_relative);

  void (*set_else)();
  char (*is_else_n_rst)();

  void (*var_loc)(const char * name, const char * map);
  char (*var_no_loc)(const char * map, const char * name,
    const char * map_name);
  CMAP_PART_VAR_RET (*get_map)(const char * map, const char * name,
    char * next_name);
  CMAP_STRINGS * (*get_vars_def)();
  CMAP_STRINGS * (*get_params)();

  void (*fn_arg_name)(char * name);
  void (*delete_fn_arg_names)();
} CMAP_PART_PUBLIC;

extern const CMAP_PART_PUBLIC cmap_part_public;

#endif
