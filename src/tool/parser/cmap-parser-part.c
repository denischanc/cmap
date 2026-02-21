
#include "cmap-parser-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-parser-params.h"
#include "cmap-parser-util.h"
#include "cmap-parser-var.h"
#include "cmap-strings.h"
#include "cmap-parser-this-args.h"
#include "cmap-config.h"

/*******************************************************************************
*******************************************************************************/

#define APPEND_ARGS_IMPL(part) \
static void append_args_##part(const char * txt, ...) \
{ \
  va_list args; \
  va_start(args, txt); \
  cmap_string_vappend_args(cmap_part_##part(), txt, args); \
  va_end(args); \
}

APPEND_ARGS_IMPL(includes)
APPEND_ARGS_IMPL(main)
APPEND_ARGS_IMPL(headers)
APPEND_ARGS_IMPL(instructions)

/*******************************************************************************
*******************************************************************************/

#define APPEND_IMPL(part) \
static void append_##part(const char * txt) \
{ \
  cmap_string_append(cmap_part_##part(), txt); \
}

APPEND_IMPL(main)
APPEND_IMPL(instructions)

void cmap_parser_part_append_instructions(const char * txt)
{
  append_instructions(txt);
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_part_include(char * includes)
{
  append_args_includes("%s\n", includes);
  free(includes);
}

/*******************************************************************************
*******************************************************************************/

static char * function_c_params(char * name, char is_static, char is_inline)
{
  char is_return = cmap_part_is_return();

  cmap_parser_params_clone();

  char * instructions = cmap_part_pop_instructions();

  CMAP_PARSER_PARAMS_RET params_ret = cmap_parser_params_get();

  append_args_main("%s%s%s %s(CMAP_PROC_CTX * proc_ctx%s)\n{\n",
    is_static ? "static " : "", is_inline ? "inline " : "",
    is_return ? "CMAP_MAP *" : "void", name, params_ret.decl);

  if(!is_static) append_args_headers("%s %s(CMAP_PROC_CTX * proc_ctx%s);\n",
    is_return ? "CMAP_MAP *" : "void", name, params_ret.decl);

  free(name);

  append_main(instructions);
  free(instructions);

  append_main("}\n\n");

  char * params_impl = strdup(params_ret.impl);
  cmap_parser_params_delete(params_ret);
  return params_impl;
}

void cmap_parser_part_function_c(char * name, char is_static)
{
  free(function_c_params(name, is_static, (1 == 0)));
}

void cmap_parser_part_instructions_root()
{
  cmap_parser_part_function_c(strdup(cmap_config_fn()),
    cmap_config_is_only_c());
}

/*******************************************************************************
*******************************************************************************/

static void function_def_apply(const char * string, void * data)
{
  const char * map_def = (const char *)data;
  char * var_name = cmap_parser_var_name(strdup(string));

  APPEND_INSTRUCTION_ARGS(
    "cmap_set(%s, \"%s\", %s);", map_def, string, var_name);

  free(var_name);
}

static void function_def(char * map_name, CMAP_STRINGS * vars_def)
{
  char * map_def = NEXT_NAME_DEFS();

  PREPEND_MAP_VAR(map_def);
  APPEND_INSTRUCTION_ARGS(
    "%s = cmap_fn_require_definitions((CMAP_FN *)%s, proc_ctx);",
    map_def, map_name);

  cmap_strings_apply(vars_def, function_def_apply, map_def);

  free(map_def);
}

char * cmap_parser_part_function(char * fn_name)
{
  char * map_name = NEXT_NAME_FN();
  CMAP_STRINGS * vars_def = NULL;

  if(fn_name == NULL)
  {
    cmap_strings_add_all(&vars_def, cmap_part_get_vars_def());

    fn_name = NEXT_NAME_PROCESS();

    append_args_main("static CMAP_MAP * %s(CMAP_PROC_CTX * proc_ctx, %s)\n{\n",
      fn_name, cmap_parser_this_args_decl());

    if(!cmap_part_is_return()) APPEND_INSTRUCTION("return NULL;");
    char * instructions = cmap_part_pop_instructions();
    cmap_part_delete_fn_arg_names();
    append_main(instructions);
    free(instructions);

    append_main("}\n\n");
  }

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS(
    "%s = (CMAP_MAP *)cmap_fn(%s, proc_ctx);", map_name, fn_name);

  free(fn_name);

  if(vars_def != NULL)
  {
    function_def(map_name, vars_def);
    cmap_strings_delete(&vars_def);
  }

  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

char * cmap_parser_part_function_cmp()
{
  cmap_parser_params_clone();

  char * bool_fn_name = NEXT_NAME_CMP(),
    * instructions = cmap_part_pop_instructions();

  CMAP_PARSER_PARAMS_RET params_ret = cmap_parser_params_get();

  append_args_main("static inline char %s(CMAP_PROC_CTX * proc_ctx%s)\n{\n",
    bool_fn_name, params_ret.decl);
  append_main(instructions);
  free(instructions);
  append_main("}\n\n");

  cmap_string_append_args(&bool_fn_name, "(proc_ctx%s)", params_ret.impl);

  cmap_parser_params_delete(params_ret);

  return bool_fn_name;
}

/*******************************************************************************
*******************************************************************************/

void cmap_parser_part_c_impl(char * impl)
{
  append_args_instructions("  %s\n\n", impl);
  free(impl);
}

void cmap_parser_part_c_impl_root(char * impl)
{
  append_main(impl);
  append_main("\n\n");
  free(impl);
}

/*******************************************************************************
*******************************************************************************/

char * cmap_parser_part_for_iter()
{
  char * call = NEXT_NAME_FOR_ITER();

  char * params_impl = function_c_params(strdup(call), (1 == 1), (1 == 1));
  cmap_string_append_args(&call, "(proc_ctx%s)", params_impl);
  free(params_impl);

  return call;
}
