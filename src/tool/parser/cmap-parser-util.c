
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

static void append_instruction_args(const char * txt, ...)
{
  char * instruction = NULL;

  va_list args;
  va_start(args, txt);
  cmap_string_public.vappend_args(&instruction, txt, args);
  va_end(args);

  cmap_part_public.add_instruction(instruction);

  free(instruction);
}

static void append_variable_args(const char * txt, ...)
{
  char * variable = NULL;

  va_list args;
  va_start(args, txt);
  cmap_string_public.vappend_args(&variable, txt, args);
  va_end(args);

  cmap_part_public.add_variable(variable);

  free(variable);
}

static void prepend_instruction_args(const char * txt, ...)
{
  char * instruction = NULL;

  va_list args;
  va_start(args, txt);
  cmap_string_public.vappend_args(&instruction, txt, args);
  va_end(args);

  cmap_part_public.prepend_instruction(instruction);

  free(instruction);
}

static void append_instruction(const char * txt)
{
  cmap_part_public.add_instruction(txt);
}

static void append_lf()
{
  cmap_part_public.add_lf();
}

/*******************************************************************************
*******************************************************************************/

static void add_args(char ** instruction, char * args)
{
  if(args != NULL)
  {
    cmap_string_public.append(instruction, args);
    free(args);
  }
  cmap_string_public.append(instruction, ", NULL);");
}

static void append_instruction_args_args(char * args, const char * txt, ...)
{
  char * instruction = NULL;

  va_list args_;
  va_start(args_, txt);
  cmap_string_public.vappend_args(&instruction, txt, args_);
  va_end(args_);

  add_args(&instruction, args);

  cmap_part_public.add_instruction(instruction);

  free(instruction);
}

/*******************************************************************************
*******************************************************************************/

static const char * add_definitions()
{
  if(!cmap_part_public.ctx.is_definitions_n_set())
  {
    CMAP_PART_CTX * ctx_bup = cmap_part_ctx_public.bup(
      cmap_part_ctx_public.fn_c());
    append_variable_args(
      "CMAP_MAP * %s = cmap_definitions(proc_ctx);", DEFINITIONS_VAR_NAME);
    cmap_part_ctx_public.restore(ctx_bup);
  }
  return DEFINITIONS_VAR_NAME;
}

static const char * add_global_env()
{
  if(!cmap_part_public.ctx.is_global_env_n_set())
  {
    CMAP_PART_CTX * ctx_bup = cmap_part_ctx_public.bup(
      cmap_part_ctx_public.fn_c());
    append_variable_args(
      "CMAP_MAP * %s = cmap_global_env(proc_ctx);", GLOBAL_ENV_VAR_NAME);
    cmap_part_ctx_public.restore(ctx_bup);
  }
  return GLOBAL_ENV_VAR_NAME;
}

/*******************************************************************************
*******************************************************************************/

static void prepend_map_var(const char * map_name)
{
  append_variable_args("CMAP_MAP * %s = NULL;", map_name);
}

/*******************************************************************************
*******************************************************************************/

static char * next_name(const char * what)
{
  char * name = NULL;
  cmap_string_public.append_args(&name, "cmap_gen_%s_id%s", what,
    cmap_part_public.ctx.uid());
  return name;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public =
{
  append_instruction_args, append_variable_args, prepend_instruction_args,
  append_instruction, append_lf,
  add_args, append_instruction_args_args,
  add_definitions, add_global_env,
  prepend_map_var,
  next_name
};
