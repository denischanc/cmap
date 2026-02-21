
#include "cmap-parser-util.h"

#include <stdlib.h>
#include "cmap-string.h"
#include "cmap-part.h"
#include "cmap-part-ctx.h"

/*******************************************************************************
*******************************************************************************/

static const char * DEFINITIONS_VAR_NAME = "cmap_definitions_",
  * GLOBAL_ENV_VAR_NAME = "cmap_global_env_";

/*******************************************************************************
*******************************************************************************/

void cmap_parser_util_append_instruction_args(const char * txt, ...)
{
  char * instruction = NULL;

  va_list args;
  va_start(args, txt);
  cmap_string_vappend_args(&instruction, txt, args);
  va_end(args);

  cmap_part_add_instruction(instruction);

  free(instruction);
}

void cmap_parser_util_append_variable_args(const char * txt, ...)
{
  char * variable = NULL;

  va_list args;
  va_start(args, txt);
  cmap_string_vappend_args(&variable, txt, args);
  va_end(args);

  cmap_part_add_variable(variable);

  free(variable);
}

void cmap_parser_util_prepend_instruction_args(const char * txt, ...)
{
  char * instruction = NULL;

  va_list args;
  va_start(args, txt);
  cmap_string_vappend_args(&instruction, txt, args);
  va_end(args);

  cmap_part_prepend_instruction(instruction);

  free(instruction);
}

void cmap_parser_util_append_instruction(const char * txt)
{
  cmap_part_add_instruction(txt);
}

void cmap_parser_util_append_lf()
{
  cmap_part_add_lf();
}

/*******************************************************************************
*******************************************************************************/

static void add_args(char ** instruction, char * args)
{
  if(args != NULL)
  {
    cmap_string_append(instruction, args);
    free(args);
  }
  cmap_string_append(instruction, ", NULL);");
}

void cmap_parser_util_append_instruction_args_args(char * args,
  const char * txt, ...)
{
  char * instruction = NULL;

  va_list args_;
  va_start(args_, txt);
  cmap_string_vappend_args(&instruction, txt, args_);
  va_end(args_);

  add_args(&instruction, args);

  cmap_part_add_instruction(instruction);

  free(instruction);
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_parser_util_add_definitions()
{
  if(!cmap_part_is_definitions_n_set())
  {
    CMAP_PART_CTX * ctx_split = cmap_part_ctx_split(cmap_part_ctx_fn_c());
    cmap_parser_util_append_variable_args(
      "CMAP_MAP * %s = cmap_definitions(proc_ctx);", DEFINITIONS_VAR_NAME);
    cmap_part_ctx_join(ctx_split);
  }
  return DEFINITIONS_VAR_NAME;
}

const char * cmap_parser_util_add_global_env()
{
  if(!cmap_part_is_global_env_n_set())
  {
    CMAP_PART_CTX * ctx_split = cmap_part_ctx_split(cmap_part_ctx_fn_c());
    cmap_parser_util_append_variable_args(
      "CMAP_MAP * %s = cmap_global_env(proc_ctx);", GLOBAL_ENV_VAR_NAME);
    cmap_part_ctx_join(ctx_split);
  }
  return GLOBAL_ENV_VAR_NAME;
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_util_prepend_map_var(const char * map_name)
{
  cmap_parser_util_append_variable_args("CMAP_MAP * %s = NULL;", map_name);
}

/*******************************************************************************
*******************************************************************************/

char * cmap_parser_util_next_name(const char * what)
{
  char * name = NULL;
  cmap_string_append_args(&name, "cmap_gen_%s_id%s", what, cmap_part_uid());
  return name;
}
