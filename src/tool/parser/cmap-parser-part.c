
#include "cmap-parser-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-parser-util.h"
#include "cmap-fn-name.h"
#include "cmap-parser-var.h"
#include "cmap-strings.h"

/*******************************************************************************
*******************************************************************************/

#define APPEND_ARGS_IMPL(part) \
static void append_args_##part(const char * txt, ...) \
{ \
  va_list args; \
  va_start(args, txt); \
  cmap_string_public.vappend_args(cmap_part_public.part(), txt, args); \
  va_end(args); \
}

CMAP_PARSER_PART_LOOP(APPEND_ARGS_IMPL)

/*******************************************************************************
*******************************************************************************/

#define APPEND_IMPL(part) \
static void append_##part(const char * txt) \
{ \
  cmap_string_public.append(cmap_part_public.part(), txt); \
}

CMAP_PARSER_PART_LOOP(APPEND_IMPL)

/*******************************************************************************
*******************************************************************************/

static void include_(char * includes)
{
  append_args_includes("%s\n", includes);
  free(includes);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PARSER_PARAMS_RET function_c_to_part(char ** part, char * name,
  char is_static)
{
  char is_return_fn = cmap_part_public.is_return_fn(),
    is_return = cmap_part_public.is_return();

  cmap_parser_params_public.clone();

  if(is_return_fn && !is_return) APPEND_INSTRUCTION("return NULL;");
  char * instructions = cmap_part_public.pop_instructions();

  CMAP_PARSER_PARAMS_RET params_ret = cmap_parser_params_public.get();

  cmap_string_public.append_args(part,
    "%s%s %s(CMAP_PROC_CTX * proc_ctx%s)\n{\n",
    is_static ? "static " : "", is_return_fn ? "CMAP_MAP *" : "void", name,
    params_ret.decl);

  if(!is_static) append_args_headers("%s %s(CMAP_PROC_CTX * proc_ctx%s);\n",
    is_return_fn ? "CMAP_MAP *" : "void", name, params_ret.decl);

  free(name);

  cmap_string_public.append(part, instructions);
  free(instructions);

  cmap_string_public.append(part, "}\n\n");

  return params_ret;
}

static void function_c(char * name, char is_static)
{
  cmap_parser_params_public.delete(
    function_c_to_part(cmap_part_public.main(), name, is_static));
}

static void instructions_root()
{
  function_c(strdup(cmap_fn_name_public.name()), (1 == 0));
}

/*******************************************************************************
*******************************************************************************/

static void function_def_apply(const char * string, void * data)
{
  const char * map_def = (const char *)data;
  char * var_name = cmap_parser_var_public.name(strdup(string));

  APPEND_INSTRUCTION_ARGS(
    "cmap_set(%s, \"%s\", %s);", map_def, string, var_name);

  free(var_name);
}

static void function_def(char * map_name, CMAP_STRINGS * vars_def)
{
  char * map_def = NEXT_NAME("defs");

  PREPEND_MAP_VAR(map_def);
  APPEND_INSTRUCTION_ARGS(
    "%s = cmap_fn_require_definitions((CMAP_FN *)%s, proc_ctx);",
    map_def, map_name);

  cmap_strings_public.apply(vars_def, function_def_apply, map_def);

  free(map_def);
}

static char * function(char * fn_name)
{
  char * map_name = NEXT_NAME("fn");
  CMAP_STRINGS * vars_def = NULL;

  if(fn_name == NULL)
  {
    cmap_strings_public.add_all(&vars_def, cmap_part_public.get_vars_def());

    fn_name = NEXT_NAME("process");

    append_args_functions("static CMAP_MAP * %s(CMAP_PROC_CTX * proc_ctx,\n"
      "  CMAP_MAP * this, CMAP_LIST * args)\n{\n", fn_name);

    if(!cmap_part_public.is_return()) APPEND_INSTRUCTION("return NULL;");
    cmap_part_public.pop_instructions_to_part(cmap_part_public.functions());
    cmap_part_public.delete_fn_arg_names();

    append_functions("}\n\n");
  }

  PREPEND_MAP_VAR(map_name);
  APPEND_INSTRUCTION_ARGS(
    "%s = (CMAP_MAP *)cmap_fn(%s, proc_ctx);", map_name, fn_name);

  free(fn_name);

  if(vars_def != NULL)
  {
    function_def(map_name, vars_def);
    cmap_strings_public.delete(&vars_def);
  }

  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void c_impl(char * impl)
{
  append_args_instructions("  %s\n\n", impl);
  free(impl);
}

static void c_impl_root(char * impl)
{
  append_main(impl);
  append_main("\n\n");
  free(impl);
}

/*******************************************************************************
*******************************************************************************/

#define APPEND_SET(part) append_args_##part, append_##part,

const CMAP_PARSER_PART_PUBLIC cmap_parser_part_public =
{
  CMAP_PARSER_PART_LOOP(APPEND_SET)
  include_, function_c_to_part, function_c, instructions_root,
  function,
  c_impl, c_impl_root
};
