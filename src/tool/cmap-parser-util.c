
#include "cmap-parser-util.h"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "cmap-string.h"
#include "cmap-part.h"
#include "cmap-fn-name.h"
#include "cmap-parser-util-params.h"
#include "cmap-parser-util-argnames.h"

/*******************************************************************************
*******************************************************************************/

#define APPEND_ARGS_IMPL(part) \
__attribute__((unused)) static void append_args_##part(const char * txt, ...) \
{ \
  va_list args; \
  va_start(args, txt); \
  cmap_string_public.vappend_args(cmap_part_public.part(), txt, args); \
  va_end(args); \
}

CMAP_PART_LOOP(APPEND_ARGS_IMPL)
APPEND_ARGS_IMPL(instructions)

/*******************************************************************************
*******************************************************************************/

#define APPEND_IMPL(part) \
__attribute__((unused)) static void append_##part(const char * txt) \
{ \
  cmap_string_public.append(cmap_part_public.part(), txt); \
}

CMAP_PART_LOOP(APPEND_IMPL)
APPEND_IMPL(instructions)

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

static const char * DEFINITIONS_VAR_NAME = "cmap_definitions_";
static const char * GLOBAL_ENV_VAR_NAME = "cmap_global_env_";

static int id = 1;

/*******************************************************************************
*******************************************************************************/

static char * next_name(const char * what)
{
  char * name = NULL;
  cmap_string_public.append_args(&name, "cmap_gen_%s_id%d", what, id++);
  return name;
}

/*******************************************************************************
*******************************************************************************/

static const char * add_definitions()
{
  if(!cmap_part_public.is_definitions()) append_variable_args(
    "CMAP_MAP * %s = cmap_definitions(proc_ctx);", DEFINITIONS_VAR_NAME);
  return DEFINITIONS_VAR_NAME;
}

/*******************************************************************************
*******************************************************************************/

static const char * add_global_env()
{
  if(!cmap_part_public.is_global_env()) append_variable_args(
    "CMAP_MAP * %s = cmap_global_env(proc_ctx);", GLOBAL_ENV_VAR_NAME);
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

static void include_(char * includes)
{
  append_args_includes("%s\n", includes);
  free(includes);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PARSER_UTIL_PARAMS_RET function_c_to_part(char ** part,
  char * name, char is_static)
{
  char is_return_fn = cmap_part_public.is_return_fn(),
    is_return = cmap_part_public.is_return();

  cmap_parser_util_params_public.clone();

  if(is_return_fn && !is_return) append_instruction("return NULL;");
  char * instructions = cmap_part_public.pop_instructions();

  CMAP_PARSER_UTIL_PARAMS_RET params_ret = cmap_parser_util_params_public.get();

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
  cmap_parser_util_params_public.delete(
    function_c_to_part(cmap_part_public.main(), name, is_static));
}

static void instructions_root()
{
  function_c(strdup(cmap_fn_name_public.name()), (1 == 0));
}

/*******************************************************************************
*******************************************************************************/

static char * path(char * map, char * name)
{
  char * map_name = next_name("var");
  CMAP_PART_VAR_RET ret = cmap_part_public.get_map(map, name, map_name);

  if((ret.ret.map == map_name) || !ret.ret.affected)
  {
    char * instruction = strdup("");

    if(ret.ret.map == map_name) prepend_map_var(map_name);
    else cmap_string_public.append_args(&instruction, "if(%s == NULL) ",
      ret.ret.map);

    const char * dst = map;
    if(dst == NULL) dst = ret.is_def ? add_definitions() : add_global_env();
    append_instruction_args("%s%s = cmap_get(%s, \"%s\");",
      instruction, ret.ret.map, dst, name);
    append_lf();

    free(instruction);
  }

  free(map);
  free(name);

  if(map_name != ret.ret.map) map_name = strdup(ret.ret.map);
  return map_name;
}

static char * name(char * name)
{
  return path(NULL, name);
}

/*******************************************************************************
*******************************************************************************/

static void set_local(char * name, char * map)
{
  cmap_part_public.var_loc(name, map);

  append_instruction_args("cmap_set(%s, \"%s\", %s);",
    add_definitions(), name, map);
  append_lf();

  free(name);
  free(map);
}

static void set_fn_arg_name(char * name, int off)
{
  cmap_part_public.var_loc(name, name);

  append_variable_args("CMAP_MAP * %s = cmap_list_get(args, %d);", name, off);
  prepend_instruction_args("cmap_set(%s, \"%s\", %s);",
    add_definitions(), name, name);

  free(name);
}

/*******************************************************************************
*******************************************************************************/

static void set_path(char * src, char * name, char * map)
{
  char is_def = cmap_part_public.var_no_loc(src, name, map);
  const char * dst = src;
  if(dst == NULL) dst = is_def ? add_definitions() : add_global_env();

  append_instruction_args("cmap_set(%s, \"%s\", %s);", dst, name, map);
  append_lf();

  free(src);
  free(name);
  free(map);
}

static void set_global(char * name, char * map)
{
  set_path(NULL, name, map);
}

/*******************************************************************************
*******************************************************************************/

static char * args(char * map)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, ", %s", map);

  free(map);

  return ret;
}

static char * args_push(char * list, char * map)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "%s, %s", list, map);

  free(list);
  free(map);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void arg_name(char * name)
{
  cmap_part_public.fn_arg_name(name);
  free(name);
}

/*******************************************************************************
*******************************************************************************/

static char * args_map(char * name, char * map)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, ", \"%s\", %s", name, map);

  free(name);
  free(map);

  return ret;
}

static char * args_map_push(char * list, char * name, char * map)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "%s, \"%s\", %s", list, name, map);

  free(list);
  free(name);
  free(map);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * map_args(char * args)
{
  char * map_name = next_name("map");

  prepend_map_var(map_name);
  append_instruction_args_args(args, "%s = cmap_to_map(proc_ctx", map_name);
  append_lf();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * list_args(char * args)
{
  char * map_name = next_name("list");

  prepend_map_var(map_name);
  append_instruction_args_args(args,
    "%s = (CMAP_MAP *)cmap_to_list(proc_ctx", map_name);
  append_lf();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * string(char * string)
{
  char * map_name = next_name("string");

  prepend_map_var(map_name);
  append_instruction_args(
    "%s = (CMAP_MAP *)cmap_string(%s, 0, proc_ctx);", map_name, string);
  append_lf();

  free(string);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * int_(char * i)
{
  char * map_name = next_name("int");

  prepend_map_var(map_name);
  append_instruction_args(
    "%s = (CMAP_MAP *)cmap_int(%s, proc_ctx);", map_name, i);
  append_lf();

  free(i);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void return_(char * map)
{
  if(map == NULL)
  {
    if(cmap_part_public.is_return_fn()) append_instruction("return NULL;");
    else append_instruction("return;");
  }
  else
  {
    append_instruction_args("return %s;", map);
    free(map);

    cmap_part_public.return_fn();
    cmap_part_public.return_();
  }
}

/*******************************************************************************
*******************************************************************************/

static char * process(char * map, char * fn_name, char * args)
{
  char * map_keep = (map == NULL) ? strdup("NULL") : strdup(map),
    * map_fn = (map == NULL) ? name(fn_name) : path(map, fn_name),
    * instruction = NULL;

  cmap_string_public.append_args(&instruction,
    "cmap_fn_proc((CMAP_FN *)%s, proc_ctx, %s", map_fn, map_keep);
  add_args(&instruction, args);

  free(map_fn);
  free(map_keep);

  return instruction;
}

/*******************************************************************************
*******************************************************************************/

static char * process_fn(char * fn, char * args)
{
  char * instruction = NULL;

  cmap_string_public.append_args(&instruction,
    "cmap_fn_proc((CMAP_FN *)%s, proc_ctx, NULL", fn);
  add_args(&instruction, args);

  free(fn);

  return instruction;
}

/*******************************************************************************
*******************************************************************************/

static char * process_instruction(char * txt, char need_ret)
{
  char * map_name = NULL;

  if(need_ret)
  {
    map_name = next_name("var");
    prepend_map_var(map_name);

    append_instruction_args("%s = %s", map_name, txt);
  }
  else append_instruction(txt);
  append_lf();

  free(txt);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * process_c(char * fn_name, char need_ret)
{
  char * map_name = NULL;

  char * proc_ctx_name = next_name("proc_ctx");
  append_instruction_args("CMAP_PROC_CTX * %s = cmap_proc_ctx(proc_ctx);",
    proc_ctx_name);

  if(need_ret)
  {
    map_name = next_name("var");
    prepend_map_var(map_name);
    append_instruction_args("%s = cmap_delete_proc_ctx(%s, %s(%s));",
      map_name, proc_ctx_name, fn_name, proc_ctx_name);
  }
  else
  {
    append_instruction_args("%s(%s);", fn_name, proc_ctx_name);
    append_instruction_args("cmap_delete_proc_ctx(%s, NULL);", proc_ctx_name);
  }
  append_lf();

  free(fn_name);
  free(proc_ctx_name);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void function_def_apply(const char * string, void * data)
{
  const char * map_def = (const char *)data;
  char * var_name = name(strdup(string));

  append_instruction_args("cmap_set(%s, \"%s\", %s);",
    map_def, string, var_name);

  free(var_name);
}

static void function_def(char * map_name, CMAP_STRINGS * vars_def)
{
  char * map_def = next_name("defs");

  prepend_map_var(map_def);
  append_instruction_args(
    "%s = cmap_fn_require_definitions((CMAP_FN *)%s, proc_ctx);",
    map_def, map_name);

  cmap_strings_public.apply(vars_def, function_def_apply, map_def);

  free(map_def);
}

static char * function(char * fn_name)
{
  char * map_name = next_name("fn");
  CMAP_STRINGS * vars_def = NULL;

  if(fn_name == NULL)
  {
    cmap_strings_public.add_all(&vars_def, cmap_part_public.get_vars_def());

    fn_name = next_name("process");

    append_args_functions(
      "static CMAP_MAP * %s(CMAP_PROC_CTX * proc_ctx,\n"
      "  CMAP_MAP * this, CMAP_LIST * args)\n{\n", fn_name);

    if(!cmap_part_public.is_return()) append_instruction("return NULL;");
    cmap_part_public.pop_instructions_to_part(cmap_part_public.functions());

    append_functions("}\n\n");
  }

  prepend_map_var(map_name);
  append_instruction_args(
    "%s = (CMAP_MAP *)cmap_fn(%s, proc_ctx);", map_name, fn_name);

  free(fn_name);

  if(vars_def != NULL)
  {
    function_def(map_name, vars_def);
    cmap_strings_public.delete(&vars_def);
  }

  char * args = cmap_parser_util_argnames_public.get();
  if(args != NULL) append_instruction_args_args(args,
    "cmap_add_arg_names((CMAP_FN *)%s", map_name);

  append_lf();

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

static void if_(char * cmp_call)
{
  char * instructions = cmap_part_public.pop_instructions();

  char * else_ = cmap_part_public.is_else_n_rst() ? "else " : "";
  append_instruction_args("%sif(%s)", else_, cmp_call);
  free(cmp_call);
  append_instruction("{");
  append_instructions(instructions);
  free(instructions);
  append_instruction("}");
}

static void else_empty()
{
  append_lf();
}

static void else_if()
{
  cmap_part_public.set_else();
}

static void else_()
{
  char * instructions = cmap_part_public.pop_instructions();

  append_instruction("else");
  append_instruction("{");
  append_instructions(instructions);
  free(instructions);
  append_instruction("}");
  append_lf();
}

/*******************************************************************************
*******************************************************************************/

static char * cmp_to_bool_fn(char * cmp_name, const char * op)
{
  append_instruction_args("return (%s %s 0);", cmp_name, op);
  free(cmp_name);

  cmap_parser_util_params_public.clone();

  char * bool_fn_name = next_name("cmp"),
    * instructions = cmap_part_public.pop_instructions();

  CMAP_PARSER_UTIL_PARAMS_RET params_ret = cmap_parser_util_params_public.get();

  append_args_functions(
    "static inline char %s(CMAP_PROC_CTX * proc_ctx%s)\n{\n",
    bool_fn_name, params_ret.decl);
  append_functions(instructions);
  free(instructions);
  append_functions("}\n\n");

  cmap_string_public.append_args(&bool_fn_name, "(proc_ctx%s)",
    params_ret.impl);

  cmap_parser_util_params_public.delete(params_ret);

  return bool_fn_name;
}

#define CMP_IMPL(name, op) \
static char * name(char * map_l, char * map_r) \
{ \
  char * cmp_name = next_name("int_" #name); \
 \
  append_instruction_args("int %s = cmap_cmp(%s, %s);", \
    cmp_name, map_l, map_r); \
  free(map_l); \
  free(map_r); \
  append_lf(); \
 \
  return cmp_to_bool_fn(cmp_name, #op); \
}

CMAP_PARSER_UTIL_CMP_LOOP(CMP_IMPL)

static char * cmp_unique(char * map)
{
  char * cmp_name = next_name("int");

  append_instruction_args("int %s;", cmp_name);
  append_instruction_args("if(cmap_nature(%s) == CMAP_INT_NATURE)", map);
  append_instruction_args(
    SPACE "%s = (cmap_int_get((CMAP_INT *)%s) == 0) ? 0 : 1;", cmp_name, map);
  append_instruction_args("else %s = (%s == NULL) ? 0 : 1;", cmp_name, map);
  free(map);
  append_lf();

  return cmp_to_bool_fn(cmp_name, "!=");
}

/*******************************************************************************
*******************************************************************************/

static char * new(char * map, char * args)
{
  char * map_name = next_name("var");

  prepend_map_var(map_name);
  append_instruction_args_args(args,
    "%s = cmap_new((CMAP_FN *)%s, proc_ctx", map_name, map);
  append_lf();

  free(map);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void set_sb_int(char * map, char * i, char * map_src)
{
  append_instruction_args(
    "cmap_list_set((CMAP_LIST *)%s, %s, %s);", map, i, map_src);
  append_lf();

  free(map);
  free(i);
  free(map_src);
}

static void set_sb_string(char * map, char * string, char * map_src)
{
  append_instruction_args(
    "cmap_set(%s, %s, %s);", map, string, map_src);
  append_lf();

  free(map);
  free(string);
  free(map_src);
}

static void set_sb_map(char * map, char * map_i, char * map_src)
{
  append_instruction_args("if(cmap_nature(%s) == CMAP_INT_NATURE)", map_i);
  append_instruction_args(
    SPACE "cmap_list_set((CMAP_LIST *)%s, cmap_int_get((CMAP_INT *)%s), %s);",
    map, map_i, map_src);
  append_instruction_args(
    "else if(cmap_nature(%s) == CMAP_STRING_NATURE)", map_i);
  append_instruction_args(
    SPACE "cmap_set(%s, cmap_string_val((CMAP_STRING *)%s), %s);",
    map, map_i, map_src);
  append_lf();

  free(map);
  free(map_i);
  free(map_src);
}

/*******************************************************************************
*******************************************************************************/

static char * sb_int(char * map, char * i)
{
  char * map_name = next_name("var");

  prepend_map_var(map_name);
  append_instruction_args(
    "%s = cmap_list_get((CMAP_LIST *)%s, %s);", map_name, map, i);
  append_lf();

  free(map);
  free(i);

  return map_name;
}

static char * sb_string(char * map, char * string)
{
  char * map_name = next_name("var");

  prepend_map_var(map_name);
  append_instruction_args("%s = cmap_get(%s, %s);", map_name, map, string);
  append_lf();

  free(map);
  free(string);

  return map_name;
}

static char * sb_map(char * map, char * map_i)
{
  char * map_name = next_name("var");

  prepend_map_var(map_name);
  append_instruction_args("if(cmap_nature(%s) == CMAP_INT_NATURE)", map_i);
  append_instruction_args(
    SPACE "%s = cmap_list_get((CMAP_LIST *)%s, cmap_int_get((CMAP_INT *)%s));",
    map_name, map, map_i);
  append_instruction_args(
    "else if(cmap_nature(%s) == CMAP_STRING_NATURE)", map_i);
  append_instruction_args(
    SPACE "%s = cmap_get(%s, cmap_string_val((CMAP_STRING *)%s));",
    map_name, map, map_i);
  append_lf();

  free(map);
  free(map_i);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * names(char * names, char * name)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "%s.%s", names, name);
  free(names);
  free(name);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * for_helper()
{
  char * call = next_name("process_for");

  CMAP_PARSER_UTIL_PARAMS_RET params_ret =
    function_c_to_part(cmap_part_public.functions(), strdup(call), (1 == 1));

  cmap_string_public.append_args(&call, "(proc_ctx%s)", params_ret.impl);

  cmap_parser_util_params_public.delete(params_ret);

  return call;
}

static void for_(char * cmp_call, char * loop_call)
{
  char * instructions = cmap_part_public.pop_instructions();

  append_instruction_args("for(; %s; %s)", cmp_call, loop_call);
  free(cmp_call); free(loop_call);
  append_instruction("{");
  append_instructions(instructions);
  free(instructions);
  append_instruction("}");
  append_lf();
}

/*******************************************************************************
*******************************************************************************/

static void while_(char * cmp_call)
{
  char * instructions = cmap_part_public.pop_instructions();

  append_instruction_args("while(%s)", cmp_call);
  free(cmp_call);
  append_instruction("{");
  append_instructions(instructions);
  free(instructions);
  append_instruction("}");
  append_lf();
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

#define CMP_SET(name, op) name,

const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public =
{
  include_, function_c, instructions_root,
  path, name,
  set_local, set_fn_arg_name, set_path, set_global,
  args, args_push,
  arg_name,
  args_map, args_map_push,
  map_args, list_args, string, int_,
  return_,
  process, process_fn, process_instruction, process_c,
  function,
  c_impl, c_impl_root,
  if_, else_empty, else_if, else_,
  CMAP_PARSER_UTIL_CMP_LOOP(CMP_SET) cmp_unique,
  new,
  set_sb_int, set_sb_string, set_sb_map, sb_int, sb_string, sb_map,
  names,
  for_helper, for_, while_,
  or, and
};
