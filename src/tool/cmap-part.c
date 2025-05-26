
#include "cmap-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-option.h"
#include "cmap-part-ctx.h"

/*******************************************************************************
*******************************************************************************/

#define PART_VAR(name) static char * name = NULL;

CMAP_PART_LOOP(PART_VAR)

/*******************************************************************************
*******************************************************************************/

#define PART_FN(name) \
static char ** name##_() \
{ \
  if(name == NULL) name = strdup(""); \
  return &name; \
}

CMAP_PART_LOOP(PART_FN)

/*******************************************************************************
*******************************************************************************/

static void push_instructions()
{
  cmap_part_ctx_public.push();
}

/*******************************************************************************
*******************************************************************************/

static char ** instructions_()
{
  return cmap_part_ctx_public.instructions(NULL);
}

/*******************************************************************************
*******************************************************************************/

static void add_instruction(const char * instruction)
{
  cmap_string_public.append_args(instructions_(), "%s%s\n",
    cmap_part_ctx_public.prefix(NULL), instruction);
}

static void add_variable(const char * variable)
{
  CMAP_PART_CTX * c = cmap_part_ctx_public.c();
  cmap_string_public.append_args(cmap_part_ctx_public.variables(NULL),
    "%s%s\n", cmap_part_ctx_public.prefix(c), variable);
}

/*******************************************************************************
*******************************************************************************/

static void add_lf()
{
  cmap_string_public.append(instructions_(), "\n");
}

/*******************************************************************************
*******************************************************************************/

static void prepend_instruction(const char * instruction)
{
  CMAP_PART_CTX * ctx_c = cmap_part_ctx_public.c();
  char ** instructions = cmap_part_ctx_public.instructions(ctx_c);

  char * tmp = *instructions;
  *instructions = NULL;
  cmap_string_public.append_args(instructions, SPACE "%s\n\n", instruction);
  cmap_string_public.append(instructions, tmp);
  free(tmp);
}

/*******************************************************************************
*******************************************************************************/

static char is_definitions()
{
  return cmap_part_ctx_public.is_definitions();
}

/*******************************************************************************
*******************************************************************************/

static char is_global_env()
{
  return cmap_part_ctx_public.is_global_env();
}

/*******************************************************************************
*******************************************************************************/

static char * pop_instructions()
{
  return cmap_part_ctx_public.pop();
}

static void pop_instructions_to_part(char ** part)
{
  char * instructions_to_append = pop_instructions();
  if(part == NULL) part = instructions_();
  cmap_string_public.append(part, instructions_to_append);
  free(instructions_to_append);
}

/*******************************************************************************
*******************************************************************************/

static void return_()
{
  cmap_part_ctx_public.return_();
}

static char is_return()
{
  return cmap_part_ctx_public.is_return();
}

/*******************************************************************************
*******************************************************************************/

static void return_fn()
{
  cmap_part_ctx_public.return_fn();
}

static char is_return_fn()
{
  return cmap_part_ctx_public.is_return_fn();
}

/*******************************************************************************
*******************************************************************************/

static void add_include(const char * name, char is_relative)
{
  if(!strncmp(name, "cmap", 4) || is_relative)
  {
    is_relative = is_relative || cmap_option_public.is_relative_inc();
    cmap_string_public.append_args(&includes,
      (is_relative) ? "#include \"%s\"\n" : "#include <cmap/%s>\n", name);
  }
  else cmap_string_public.append_args(&includes, "#include <%s>\n", name);
}

/*******************************************************************************
*******************************************************************************/

static void set_else()
{
  cmap_part_ctx_public.else_();
}

static char is_else_n_rst()
{
  return cmap_part_ctx_public.is_else();
}

/*******************************************************************************
*******************************************************************************/

static void var_loc(const char * name, const char * map)
{
  cmap_part_var_public.put_loc(name, map);
}

static char var_no_loc(const char * map, const char * name,
  const char * map_name)
{
  return cmap_part_var_public.put_no_loc(map, name, map_name);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_VAR_RET get_map(const char * map, const char * name,
  char * next_name)
{
  return cmap_part_var_public.get(map, name, next_name);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * get_vars_def()
{
  return *cmap_part_ctx_public.vars_def(NULL);
}

static CMAP_STRINGS * get_params()
{
  return *cmap_part_ctx_public.params(NULL);
}

/*******************************************************************************
*******************************************************************************/

static void fn_arg_name(char * name)
{
  cmap_strings_public.add(cmap_part_ctx_public.fn_arg_names(NULL), name);
}

static void delete_fn_arg_names()
{
  cmap_strings_public.delete(cmap_part_ctx_public.fn_arg_names(NULL));
}

/*******************************************************************************
*******************************************************************************/

#define PART_FREE(name) free(name); name = NULL;

static void clean()
{
  CMAP_PART_LOOP(PART_FREE)
}

/*******************************************************************************
*******************************************************************************/

#define PART_SET(name) name##_,

const CMAP_PART_PUBLIC cmap_part_public =
{
  clean,
  CMAP_PART_LOOP(PART_SET)
  push_instructions,
  instructions_,
  add_instruction, add_variable,
  add_lf,
  prepend_instruction,
  is_definitions,
  is_global_env,
  pop_instructions,
  pop_instructions_to_part,
  return_, is_return,
  return_fn, is_return_fn,
  add_include,
  set_else, is_else_n_rst,
  var_loc, var_no_loc, get_map, get_vars_def, get_params,
  fn_arg_name, delete_fn_arg_names
};
