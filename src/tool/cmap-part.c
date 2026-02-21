
#include "cmap-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

#define PART_VAR(name) static char * name = NULL;

CMAP_PART_LOOP(PART_VAR)

/*******************************************************************************
*******************************************************************************/

const char * cmap_part_uid()
{
  return cmap_part_ctx_uid();
}

/*******************************************************************************
*******************************************************************************/

void cmap_part_set_cmp_params()
{
  cmap_part_ctx_set_cmp_params();
}

void cmap_part_rst_cmp_params()
{
  cmap_part_ctx_rst_cmp_params();
}

/*******************************************************************************
*******************************************************************************/

void cmap_part_fn_arg_name(char * name)
{
  cmap_strings_add(cmap_part_ctx_fn_arg_names(NULL), name);
}

void cmap_part_delete_fn_arg_names()
{
  cmap_strings_delete(cmap_part_ctx_fn_arg_names(NULL));
}

/*******************************************************************************
*******************************************************************************/

char cmap_part_is_definitions_n_set()
{
  return cmap_part_ctx_is_definitions_n_set();
}

char cmap_part_is_global_env_n_set()
{
  return cmap_part_ctx_is_global_env_n_set();
}

/*******************************************************************************
*******************************************************************************/

void cmap_part_set_return()
{
  cmap_part_ctx_set_return();
}

char cmap_part_is_return()
{
  return cmap_part_ctx_is_return();
}

/*******************************************************************************
*******************************************************************************/

CMAP_STRINGS * cmap_part_get_params()
{
  return *cmap_part_ctx_params(NULL);
}

/*******************************************************************************
*******************************************************************************/

char cmap_part_return_fn()
{
  return cmap_part_ctx_return_fn(NULL);
}

/*******************************************************************************
*******************************************************************************/

CMAP_STRINGS * cmap_part_get_vars_def()
{
  return *cmap_part_ctx_vars_def(NULL);
}

/*******************************************************************************
*******************************************************************************/

CMAP_PART_CTX * cmap_part_bup_ctx()
{
  return cmap_part_ctx_bup();
}

void cmap_part_restore_ctx(CMAP_PART_CTX * ctx)
{
  cmap_part_ctx_restore(ctx);
}

void cmap_part_clean_ctx()
{
  cmap_part_ctx_clean();
}

/*******************************************************************************
*******************************************************************************/

void cmap_part_loc(const char * name, const char * next_name)
{
  cmap_part_var_put_loc(name, next_name);
}

char cmap_part_no_loc(const char * map, const char * name,
  const char * next_name)
{
  return cmap_part_var_put_no_loc(map, name, next_name);
}

/*******************************************************************************
*******************************************************************************/

CMAP_PART_VAR_RET cmap_part_get_map(const char * map, const char * name,
  char * next_name)
{
  return cmap_part_var_get(map, name, next_name);
}

/*******************************************************************************
*******************************************************************************/

void cmap_part_clean_after_proc()
{
  cmap_part_name2map_clean_after_proc();
}

/*******************************************************************************
*******************************************************************************/

#define PART_FREE(name) free(name); name = NULL;

void cmap_part_clean()
{
  CMAP_PART_LOOP(PART_FREE)
}

/*******************************************************************************
*******************************************************************************/

#define PART_FN(name) \
char ** cmap_part_##name() \
{ \
  if(name == NULL) name = strdup(""); \
  return &name; \
}

CMAP_PART_LOOP(PART_FN)

/*******************************************************************************
*******************************************************************************/

#define NATURE_FN(NAME, name, val) \
void cmap_part_nature_ctx_##name() \
{ \
  cmap_part_ctx_nature_##name(); \
}

CMAP_PART_CTX_NATURE_LOOP(NATURE_FN)

/*******************************************************************************
*******************************************************************************/

void cmap_part_push_instructions()
{
  cmap_part_ctx_push();
}

/*******************************************************************************
*******************************************************************************/

char ** cmap_part_instructions()
{
  return cmap_part_ctx_instructions(NULL);
}

/*******************************************************************************
*******************************************************************************/

void cmap_part_add_instruction(const char * instruction)
{
  cmap_string_append_args(cmap_part_instructions(), "%s%s\n",
    cmap_part_ctx_prefix(NULL), instruction);
}

void cmap_part_add_variable(const char * variable)
{
  CMAP_PART_CTX * c = cmap_part_ctx_c();
  cmap_string_append_args(cmap_part_ctx_variables(NULL),
    "%s%s\n", cmap_part_ctx_prefix(c), variable);
}

/*******************************************************************************
*******************************************************************************/

void cmap_part_add_lf()
{
  cmap_string_append(cmap_part_instructions(), "\n");
}

/*******************************************************************************
*******************************************************************************/

void cmap_part_prepend_instruction(const char * instruction)
{
  CMAP_PART_CTX * ctx_c = cmap_part_ctx_c();
  char ** instructions = cmap_part_ctx_instructions(ctx_c);

  char * tmp = *instructions;
  *instructions = NULL;
  cmap_string_append_args(instructions, "%s%s\n\n",
    cmap_part_ctx_prefix(ctx_c), instruction);
  cmap_string_append(instructions, tmp);
  free(tmp);
}

/*******************************************************************************
*******************************************************************************/

char * cmap_part_pop_instructions()
{
  return cmap_part_ctx_pop();
}

/*******************************************************************************
*******************************************************************************/

void cmap_part_add_include(const char * name, char is_relative)
{
  if(!strncmp(name, "cmap", 4) || is_relative)
  {
    is_relative = is_relative || cmap_config_is_relative_inc();
    cmap_string_append_args(&includes,
      (is_relative) ? "#include \"%s\"\n" : "#include <cmap/%s>\n", name);
  }
  else cmap_string_append_args(&includes, "#include <%s>\n", name);
}
