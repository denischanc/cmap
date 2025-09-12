
#include "cmap-parser-cmp.h"

#include <stdlib.h>
#include "cmap-parser-util.h"
#include "cmap-parser-params.h"
#include "cmap-parser-part.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

static char * cmp_to_bool_fn(char * cmp_name, const char * op)
{
  APPEND_INSTRUCTION_ARGS("return (%s %s 0);", cmp_name, op);
  free(cmp_name);

  cmap_parser_params_public.clone();

  char * bool_fn_name = NEXT_NAME("cmp"),
    * instructions = cmap_part_public.pop_instructions();

  CMAP_PARSER_PARAMS_RET params_ret = cmap_parser_params_public.get();

  cmap_parser_part_public.append_args_functions(
    "static inline char %s(CMAP_PROC_CTX * proc_ctx%s)\n{\n",
    bool_fn_name, params_ret.decl);
  cmap_parser_part_public.append_functions(instructions);
  free(instructions);
  cmap_parser_part_public.append_functions("}\n\n");

  cmap_string_public.append_args(&bool_fn_name, "(proc_ctx%s)",
    params_ret.impl);

  cmap_parser_params_public.delete(params_ret);

  return bool_fn_name;
}

/*******************************************************************************
*******************************************************************************/

#define IMPL(name, op) \
static char * name(char * map_l, char * map_r) \
{ \
  char * cmp_name = NEXT_NAME("int_" #name); \
 \
  APPEND_INSTRUCTION_ARGS( \
    "int %s = cmap_cmp(%s, %s);", cmp_name, map_l, map_r); \
  free(map_l); \
  free(map_r); \
  APPEND_LF(); \
 \
  return cmp_to_bool_fn(cmp_name, #op); \
}

CMAP_PARSER_CMP_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

static char * cmp_unique(char * map)
{
  char * cmp_name = NEXT_NAME("int");

  APPEND_INSTRUCTION_ARGS("int %s;", cmp_name);
  APPEND_INSTRUCTION_ARGS("if(cmap_nature(%s) == CMAP_INT_NATURE)", map);
  APPEND_INSTRUCTION_ARGS(
    SPACE "%s = (cmap_int_get((CMAP_INT *)%s) == 0) ? 0 : 1;", cmp_name, map);
  APPEND_INSTRUCTION_ARGS("else %s = (%s == NULL) ? 0 : 1;", cmp_name, map);
  free(map);
  APPEND_LF();

  return cmp_to_bool_fn(cmp_name, "!=");
}

/*******************************************************************************
*******************************************************************************/

static char * or_and(char * cmp_call_l, char * cmp_call_r, const char * op)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "(%s %s %s)",
    cmp_call_l, op, cmp_call_r);
  free(cmp_call_l);
  free(cmp_call_r);
  return ret;
}

static char * or(char * cmp_call_l, char * cmp_call_r)
{
  return or_and(cmp_call_l, cmp_call_r, "||");
}

static char * and(char * cmp_call_l, char * cmp_call_r)
{
  return or_and(cmp_call_l, cmp_call_r, "&&");
}

/*******************************************************************************
*******************************************************************************/

#define SET(name, op) name,

const CMAP_PARSER_CMP_PUBLIC cmap_parser_cmp_public =
{
  CMAP_PARSER_CMP_LOOP(SET) cmp_unique,
  or, and
};
