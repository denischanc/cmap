
#include "cmap-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-option.h"

/*******************************************************************************
*******************************************************************************/

#define PART_VAR(name) static char * name = NULL;

CMAP_PART_LOOP(PART_VAR)

/*******************************************************************************
*******************************************************************************/

static const char * uid()
{
  return cmap_part_ctx_public.uid();
}

/*******************************************************************************
*******************************************************************************/

static void set_cmp_params()
{
  cmap_part_ctx_public.set_cmp_params();
}

static void rst_cmp_params()
{
  cmap_part_ctx_public.rst_cmp_params();
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

static char is_definitions_n_set()
{
  return cmap_part_ctx_public.is_definitions_n_set();
}

static char is_global_env_n_set()
{
  return cmap_part_ctx_public.is_global_env_n_set();
}

/*******************************************************************************
*******************************************************************************/

static void set_return()
{
  cmap_part_ctx_public.set_return();
}

static char is_return()
{
  return cmap_part_ctx_public.is_return();
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * get_params()
{
  return *cmap_part_ctx_public.params(NULL);
}

/*******************************************************************************
*******************************************************************************/

static char return_fn()
{
  return cmap_part_ctx_public.return_fn(NULL);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * get_vars_def()
{
  return *cmap_part_ctx_public.vars_def(NULL);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX * bup()
{
  return cmap_part_ctx_public.bup(NULL);
}

static void restore(CMAP_PART_CTX * ctx)
{
  cmap_part_ctx_public.restore(ctx);
}

static void ctx_clean()
{
  cmap_part_ctx_public.clean();
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_VAR_RET loc(const char * name, const char * next_name)
{
  return cmap_part_var_public.put_loc(name, next_name);
}

static CMAP_PART_VAR_RET no_loc(const char * map, const char * name,
  const char * next_name)
{
  return cmap_part_var_public.put_no_loc(map, name, next_name);
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

static void proc_clean()
{
  cmap_part_name2map_public.proc_clean();
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

#define PART_FN(name) \
static char ** name##_() \
{ \
  if(name == NULL) name = strdup(""); \
  return &name; \
}

CMAP_PART_LOOP(PART_FN)

/*******************************************************************************
*******************************************************************************/

#define NATURE_FN(NAME, name, val) \
static void nature_ctx_##name() \
{ \
  cmap_part_ctx_public.nature_##name(); \
}

CMAP_PART_CTX_NATURE_LOOP(NATURE_FN)

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
  cmap_string_public.append_args(instructions, "%s%s\n\n",
    cmap_part_ctx_public.prefix(ctx_c), instruction);
  cmap_string_public.append(instructions, tmp);
  free(tmp);
}

/*******************************************************************************
*******************************************************************************/

static char * pop_instructions()
{
  return cmap_part_ctx_public.pop();
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

#define PART_SET(name) name##_,

#define NATURE_SET(NAME, name, val) nature_ctx_##name,

const CMAP_PART_PUBLIC cmap_part_public =
{
  {
    uid,
    set_cmp_params, rst_cmp_params,
    fn_arg_name, delete_fn_arg_names,
    is_definitions_n_set, is_global_env_n_set,
    set_return, is_return,
    get_params,
    return_fn,
    get_vars_def,
    bup, restore, ctx_clean
  },
  {
    loc, no_loc,
    get_map,
    proc_clean
  },
  clean,
  CMAP_PART_LOOP(PART_SET)
  CMAP_PART_CTX_NATURE_LOOP(NATURE_SET)
  push_instructions,
  instructions_,
  add_instruction, add_variable,
  add_lf,
  prepend_instruction,
  pop_instructions,
  add_include
};
