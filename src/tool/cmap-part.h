#ifndef __CMAP_PART_H__
#define __CMAP_PART_H__

#include "cmap-part-define.h"
#include "cmap-part-ctx.h"
#include "cmap-part-var.h"
#include "cmap-strings.h"

typedef struct
{
  const char * (*uid)();

  void (*set_cmp_params)();
  void (*rst_cmp_params)();

  void (*fn_arg_name)(char * name);
  void (*delete_fn_arg_names)();

  char (*is_definitions_n_set)();
  char (*is_global_env_n_set)();

  void (*set_return)();
  char (*is_return)();

  CMAP_STRINGS * (*get_params)();

  char (*return_fn)();

  CMAP_STRINGS * (*get_vars_def)();

  CMAP_PART_CTX * (*bup)();
  void (*restore)(CMAP_PART_CTX * ctx);
  void (*clean)();
} CMAP_PART_PUBLIC_CTX;

typedef struct
{
  void (*loc)(const char * name, const char * next_name);
  char (*no_loc)(const char * map, const char * name, const char * next_name);

  CMAP_PART_VAR_RET (*get_map)(const char * map, const char * name,
    char * next_name);

  void (*clean_after_proc)();
} CMAP_PART_PUBLIC_VAR;

typedef struct
{
  CMAP_PART_PUBLIC_CTX ctx;
  CMAP_PART_PUBLIC_VAR var;

  void (*clean)();

  CMAP_PART_LOOP(CMAP_PART_DECL)

  CMAP_PART_CTX_NATURE_LOOP(CMAP_PART_NATURE_DECL)

  void (*push_instructions)();
  char ** (*instructions)();
  void (*add_instruction)(const char * instruction);
  void (*add_variable)(const char * variable);
  void (*add_lf)();
  void (*prepend_instruction)(const char * instruction);
  char * (*pop_instructions)();

  void (*add_include)(const char * name, char is_relative);
} CMAP_PART_PUBLIC;

extern const CMAP_PART_PUBLIC cmap_part_public;

#endif
