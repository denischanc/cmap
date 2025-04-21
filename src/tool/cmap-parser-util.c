
#include "cmap-parser-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-part.h"
#include "cmap-kv.h"
#include "cmap-fn-name.h"
#include "cmap-stack-define.h"

/*******************************************************************************
*******************************************************************************/

#define APPEND_ARGS(part, txt, args...) \
  cmap_string_public.append_args(cmap_part_public.part(), txt, args)

#define APPEND(part, txt) \
  cmap_string_public.append(cmap_part_public.part(), txt)

#define APPEND_INSTRUCTION_ARGS(txt, args...) \
  cmap_string_public.append_args(&instruction, txt, args); \
  cmap_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

#define APPEND_INSTRUCTION(txt) cmap_part_public.add_instruction(txt)

#define APPEND_LF() cmap_part_public.add_lf()

#define APPEND_INSTRUCTION_ARGS_ARGS(txt, args_...) \
  cmap_string_public.append_args(&instruction, txt, args_); \
  add_args(&instruction, args); \
  cmap_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

#define PREPEND_INSTRUCTION_ARGS(txt, args...) \
  cmap_string_public.append_args(&instruction, txt, args); \
  cmap_part_public.prepend_instruction(instruction); \
  free(instruction); \
  instruction = NULL

/*******************************************************************************
*******************************************************************************/

static const char * DEFINITIONS_VAR_NAME = "definitions";
static const char * GLOBAL_ENV_VAR_NAME = "global_env";

static int id = 1;

/*******************************************************************************
*******************************************************************************/

static char * next_name()
{
  char * name = NULL;
  cmap_string_public.append_args(&name, "cmap_gen_name__%d", id++);
  return name;
}

/*******************************************************************************
*******************************************************************************/

static const char * add_definitions()
{
  if(!cmap_part_public.is_definitions())
  {
    char * instruction = NULL;
    PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s = cmap_definitions(proc_ctx);",
      DEFINITIONS_VAR_NAME);
  }
  return DEFINITIONS_VAR_NAME;
}

/*******************************************************************************
*******************************************************************************/

static const char * add_global_env()
{
  if(!cmap_part_public.is_global_env())
  {
    char * instruction = NULL;
    PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s = cmap_global_env(proc_ctx);",
      GLOBAL_ENV_VAR_NAME);
  }
  return GLOBAL_ENV_VAR_NAME;
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

/*******************************************************************************
*******************************************************************************/

static void prepend_map_var(const char * map_name)
{
  char * instruction = NULL;
  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s = NULL;", map_name);
}

/*******************************************************************************
*******************************************************************************/

CMAP_STACK_DEF(char, char)
CMAP_STACK_DEF(char_ptr, char *)

/*******************************************************************************
*******************************************************************************/

static void include_(char * includes)
{
  APPEND_ARGS(includes, "%s\n", includes);
  free(includes);
}

/*******************************************************************************
*******************************************************************************/

static void function_c_to_part(char ** part, char * name, char is_static)
{
  char is_return_fn = cmap_part_public.is_return_fn();
  cmap_string_public.append_args(part,
    "%s%s %s(CMAP_PROC_CTX * proc_ctx)\n{\n",
    is_static ? "static " : "", is_return_fn ? "CMAP_MAP *" : "void", name);

  if(!is_static)
  {
    APPEND_ARGS(headers, "%s %s(CMAP_PROC_CTX * proc_ctx);\n",
      is_return_fn ? "CMAP_MAP *" : "void", name);
  }

  free(name);

  if(is_return_fn && !cmap_part_public.is_return())
    APPEND_INSTRUCTION("return NULL;");
  cmap_part_public.pop_instructions_to_part(part);

  cmap_string_public.append(part, "}\n\n");
}

static void function_c(char * name, char is_static)
{
  function_c_to_part(cmap_part_public.main(), name, is_static);
}

static void instructions_root()
{
  function_c(strdup(cmap_fn_name_public.name()), (1 == 0));
}

/*******************************************************************************
*******************************************************************************/

static char * name(char * name)
{
  CMAP_PART_MAP_RET ret = cmap_part_public.get_map(name);
  char * instruction = NULL,
    * map_name = (ret.map == NULL) ? next_name() : strdup(ret.map);

  if((ret.map == NULL) || ret.dirty)
  {
    if(ret.map == NULL)
    {
      prepend_map_var(map_name);
      cmap_part_public.name2map(name, map_name);
    }
    else cmap_string_public.append_args(&instruction, "if(%s == NULL) ",
      map_name);

    const char * dst = ret.is_def ? add_definitions() : add_global_env();
    APPEND_INSTRUCTION_ARGS("%s = cmap_get(%s, \"%s\");", map_name, dst, name);
    APPEND_LF();
  }

  free(name);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * path(char * map, char * name)
{
  char * map_name = next_name(), * instruction = NULL;

  prepend_map_var(map_name);
  APPEND_INSTRUCTION_ARGS("%s = cmap_get(%s, \"%s\");", map_name, map, name);
  APPEND_LF();

  free(map);
  free(name);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void set_local(char * name, char * map)
{
  cmap_part_public.var_loc(name, map);

  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("cmap_set(%s, \"%s\", %s);",
    add_definitions(), name, map);
  APPEND_LF();

  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static void set_global(char * name, char * map)
{
  const char * dst = cmap_part_public.var(name, map) ?
    add_definitions() : add_global_env();

  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("cmap_set(%s, \"%s\", %s);", dst, name, map);
  APPEND_LF();

  free(name);
  free(map);
}

/*******************************************************************************
*******************************************************************************/

static void set_path(char * src, char * name, char * map)
{
  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("cmap_set(%s, \"%s\", %s);", src, name, map);
  APPEND_LF();

  free(src);
  free(name);
  free(map);
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
  char * map_name = next_name(), * instruction = NULL;

  prepend_map_var(map_name);
  APPEND_INSTRUCTION_ARGS_ARGS("%s = cmap_to_map(proc_ctx", map_name);
  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * list_args(char * args)
{
  char * map_name = next_name(), * instruction = NULL;

  prepend_map_var(map_name);
  APPEND_INSTRUCTION_ARGS_ARGS(
    "%s = (CMAP_MAP *)cmap_to_list(proc_ctx", map_name);
  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * string(char * string)
{
  char * map_name = next_name(), * instruction = NULL;

  prepend_map_var(map_name);
  APPEND_INSTRUCTION_ARGS(
    "%s = (CMAP_MAP *)cmap_string(%s, 0, proc_ctx);", map_name, string);
  APPEND_LF();

  free(string);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * int_(char * i)
{
  char * map_name = next_name(), * instruction = NULL;

  prepend_map_var(map_name);
  APPEND_INSTRUCTION_ARGS(
    "%s = (CMAP_MAP *)cmap_int(%s, proc_ctx);", map_name, i);
  APPEND_LF();

  free(i);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void return_(char * map)
{
  if(map == NULL)
  {
    if(cmap_part_public.is_return_fn()) APPEND_INSTRUCTION("return NULL;");
    else APPEND_INSTRUCTION("return;");
  }
  else
  {
    char * instruction = NULL;
    APPEND_INSTRUCTION_ARGS("return %s;", map);
    free(map);

    cmap_part_public.return_fn();
    cmap_part_public.return_();
  }
}

/*******************************************************************************
*******************************************************************************/

static char * process(char * map, char * fn_name, char * args, char need_ret)
{
  char * map_keep = (map == NULL) ? strdup("NULL") : strdup(map),
    * map_fn = (map == NULL) ? name(fn_name) : path(map, fn_name),
    * map_name = NULL, * instruction = NULL;

  if(need_ret)
  {
    map_name = next_name();
    prepend_map_var(map_name);
    APPEND_INSTRUCTION_ARGS_ARGS(
      "%s = cmap_fn_proc((CMAP_FN *)%s, proc_ctx, %s",
      map_name, map_fn, map_keep);
  }
  else
  {
    APPEND_INSTRUCTION_ARGS_ARGS("cmap_fn_proc((CMAP_FN *)%s, proc_ctx, %s",
      map_fn, map_keep);
  }
  APPEND_LF();

  free(map_fn);
  free(map_keep);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * process_fn(char * fn, char * args, char need_ret)
{
  char * map_name = NULL, * instruction = NULL;

  if(need_ret)
  {
    map_name = next_name();
    prepend_map_var(map_name);
    APPEND_INSTRUCTION_ARGS_ARGS(
      "%s = cmap_fn_proc((CMAP_FN *)%s, proc_ctx, NULL", map_name, fn);
  }
  else
  {
    APPEND_INSTRUCTION_ARGS_ARGS(
      "cmap_fn_proc((CMAP_FN *)%s, proc_ctx, NULL", fn);
  }
  APPEND_LF();

  free(fn);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

#define PROCESS_WAY_DFT 0
#define PROCESS_WAY_FN 1

static CMAP_STACK_char * process_way = NULL;
static CMAP_STACK_char_ptr * process_map_or_fn = NULL,
  * process_fn_name = NULL, * process_args = NULL;

static void process_prepare(char * map, char * fn_name, char * args)
{
  cmap_stack_char_push(&process_way, PROCESS_WAY_DFT);
  cmap_stack_char_ptr_push(&process_map_or_fn, map);
  cmap_stack_char_ptr_push(&process_fn_name, fn_name);
  cmap_stack_char_ptr_push(&process_args, args);
}

static void process_prepare_fn(char * fn, char * args)
{
  cmap_stack_char_push(&process_way, PROCESS_WAY_FN);
  cmap_stack_char_ptr_push(&process_map_or_fn, fn);
  cmap_stack_char_ptr_push(&process_args, args);
}

static char * process_resolve(char need_ret)
{
  if(cmap_stack_char_pop(&process_way) == PROCESS_WAY_FN)
  {
    return process_fn(cmap_stack_char_ptr_pop(&process_map_or_fn),
      cmap_stack_char_ptr_pop(&process_args), need_ret);
  }
  else
  {
    return process(cmap_stack_char_ptr_pop(&process_map_or_fn),
      cmap_stack_char_ptr_pop(&process_fn_name),
      cmap_stack_char_ptr_pop(&process_args), need_ret);
  }
}

/*******************************************************************************
*******************************************************************************/

static char * process_c(char * fn_name, char need_ret)
{
  char * instruction = NULL, * map_name = NULL;

  char * proc_ctx_name = next_name();
  APPEND_INSTRUCTION_ARGS(
    "CMAP_PROC_CTX * %s = cmap_proc_ctx(cmap_proc_ctx_env(proc_ctx));",
    proc_ctx_name);

  if(need_ret)
  {
    map_name = next_name();
    prepend_map_var(map_name);
    APPEND_INSTRUCTION_ARGS("%s = cmap_delete_proc_ctx(%s, %s(%s));",
      map_name, proc_ctx_name, fn_name, proc_ctx_name);
  }
  else
  {
    APPEND_INSTRUCTION_ARGS("%s(%s);", fn_name, proc_ctx_name);
    APPEND_INSTRUCTION_ARGS("cmap_delete_proc_ctx(%s, NULL);", proc_ctx_name);
  }
  APPEND_LF();

  free(fn_name);
  free(proc_ctx_name);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void function_def_apply_fn(const char * string, void * data)
{
  const char * map_def = (const char *)data;
  char * var_name = name(strdup(string)), * instruction = NULL;

  APPEND_INSTRUCTION_ARGS("cmap_set(%s, \"%s\", %s);",
    map_def, string, var_name);

  free(var_name);
}

static void function_def(char * map_name, CMAP_STRINGS * vars_def)
{
  char * map_def = next_name(), * instruction = NULL;

  prepend_map_var(map_def);
  APPEND_INSTRUCTION_ARGS(
    "%s = cmap_fn_require_definitions((CMAP_FN *)%s, proc_ctx);",
    map_def, map_name);

  cmap_strings_public.apply(vars_def, function_def_apply_fn, map_def);

  free(map_def);
}

static void arg_names_apply_fn(const char * string, void * data)
{
  cmap_string_public.append_args((char **)data, ", \"%s\"", string);
}

static char * arg_names()
{
  char * args = NULL;

  CMAP_STRINGS * arg_names_ = cmap_part_public.get_fn_arg_names();
  cmap_strings_public.apply(arg_names_, arg_names_apply_fn, &args);
  cmap_part_public.delete_fn_arg_names();

  return args;
}

static char * function(char * fn_name)
{
  char * map_name = next_name(), * instruction = NULL;
  CMAP_STRINGS * vars_def = NULL;

  if(fn_name == NULL)
  {
    vars_def = cmap_strings_public.clone(cmap_part_public.get_vars_def());

    fn_name = next_name();

    APPEND_ARGS(functions,
      "static CMAP_MAP * %s(CMAP_PROC_CTX * proc_ctx,\n"
      "  CMAP_MAP * map, CMAP_LIST * args)\n{\n", fn_name);

    if(!cmap_part_public.is_return()) APPEND_INSTRUCTION("return NULL;");
    cmap_part_public.pop_instructions_to_part(cmap_part_public.functions());

    APPEND(functions, "}\n\n");
  }

  prepend_map_var(map_name);
  APPEND_INSTRUCTION_ARGS(
    "%s = (CMAP_MAP *)cmap_fn(%s, proc_ctx);", map_name, fn_name);

  free(fn_name);

  if(vars_def != NULL)
  {
    function_def(map_name, vars_def);
    cmap_strings_public.delete(&vars_def);
  }

  char * args = arg_names();
  if(args != NULL)
  {
    APPEND_INSTRUCTION_ARGS_ARGS("cmap_add_arg_names((CMAP_FN *)%s", map_name);
  }

  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void c_impl(char * impl)
{
  APPEND_ARGS(instructions, "  %s\n\n", impl);
  free(impl);
}

static void c_impl_root(char * impl)
{
  APPEND(main, impl);
  APPEND(main, "\n\n");
  free(impl);
}

/*******************************************************************************
*******************************************************************************/

static void end_of_instructions_part()
{
  cmap_part_public.pop_instructions_to_part(NULL);

  APPEND_INSTRUCTION("}");
  APPEND_LF();
}

/*******************************************************************************
*******************************************************************************/

static void if_(char * cmp_call)
{
  char * instructions = cmap_part_public.pop_instructions(),
    * instruction = NULL;

  char * else_ = cmap_part_public.is_else_n_rst() ? "else " : "";
  APPEND_INSTRUCTION_ARGS("%sif(%s)", else_, cmp_call);
  free(cmp_call);
  APPEND_INSTRUCTION("{");
  APPEND(instructions, instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
}

static void else_empty()
{
  APPEND_LF();
}

static void else_if()
{
  cmap_part_public.set_else();
}

static void else_()
{
  char * instructions = cmap_part_public.pop_instructions();

  APPEND_INSTRUCTION("else");
  APPEND_INSTRUCTION("{");
  APPEND(instructions, instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
  APPEND_LF();
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char * decl;
  char * impl;
} CMP_PARAMS;

static void cmp_params_apply_fn(const char * string, void * data)
{
  char * map = name(strdup(string));

  CMP_PARAMS * params = (CMP_PARAMS *)data;
  cmap_string_public.append_args(&params -> decl, ", CMAP_MAP * %s", map);
  cmap_string_public.append_args(&params -> impl, ", %s", map);

  free(map);
}

static CMP_PARAMS cmp_params(CMAP_STRINGS * params)
{
  CMP_PARAMS cmp_params_;
  cmp_params_.decl = strdup("");
  cmp_params_.impl = strdup("");

  cmap_strings_public.apply(params, cmp_params_apply_fn, &cmp_params_);

  return cmp_params_;
}

static char * cmp_to_bool_fn(char * cmp_name, const char * op)
{
  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("return (%s %s 0);", cmp_name, op);
  free(cmp_name);

  CMAP_STRINGS * params =
    cmap_strings_public.clone(cmap_part_public.get_params());

  char * bool_fn_name = next_name(),
    * instructions = cmap_part_public.pop_instructions();

  CMP_PARAMS cmp_params_ = cmp_params(params);
  cmap_strings_public.delete(&params);

  APPEND_ARGS(functions,
    "static inline char %s(CMAP_PROC_CTX * proc_ctx%s)\n{\n",
    bool_fn_name, cmp_params_.decl);
  free(cmp_params_.decl);
  APPEND(functions, instructions);
  free(instructions);
  APPEND(functions, "}\n\n");

  cmap_string_public.append_args(&bool_fn_name, "(proc_ctx%s)",
    cmp_params_.impl);
  free(cmp_params_.impl);

  return bool_fn_name;
}

#define CMP_IMPL(name, op) \
static char * name(char * map_l, char * map_r) \
{ \
  char * cmp_name = next_name(), * instruction = NULL; \
 \
  APPEND_INSTRUCTION_ARGS("int %s = cmap_cmp(%s, %s);", \
    cmp_name, map_l, map_r); \
  free(map_l); \
  free(map_r); \
  APPEND_LF(); \
 \
  return cmp_to_bool_fn(cmp_name, #op); \
}

CMAP_PARSER_UTIL_CMP_LOOP(CMP_IMPL)

static char * cmp_unique(char * map)
{
  char * cmp_name = next_name(), * instruction = NULL;

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

static char * new(char * map, char * args)
{
  char * map_name = next_name(), * instruction = NULL;

  prepend_map_var(map_name);
  APPEND_INSTRUCTION_ARGS_ARGS(
    "%s = cmap_new((CMAP_FN *)%s, proc_ctx", map_name, map);
  APPEND_LF();

  free(map);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static void set_sb_int(char * map, char * i, char * map_src)
{
  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS(
    "cmap_list_set((CMAP_LIST *)%s, %s, %s);", map, i, map_src);
  APPEND_LF();

  free(map);
  free(i);
  free(map_src);
}

static void set_sb_string(char * map, char * string, char * map_src)
{
  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS(
    "cmap_set(%s, %s, %s);", map, string, map_src);
  APPEND_LF();

  free(map);
  free(string);
  free(map_src);
}

static void set_sb_map(char * map, char * map_i, char * map_src)
{
  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("if(cmap_nature(%s) == CMAP_INT_NATURE)", map_i);
  APPEND_INSTRUCTION_ARGS(
    SPACE "cmap_list_set((CMAP_LIST *)%s, cmap_int_get((CMAP_INT *)%s), %s);",
    map, map_i, map_src);
  APPEND_INSTRUCTION_ARGS(
    "else if(cmap_nature(%s) == CMAP_STRING_NATURE)", map_i);
  APPEND_INSTRUCTION_ARGS(
    SPACE "cmap_set(%s, cmap_string_val((CMAP_STRING *)%s), %s);",
    map, map_i, map_src);
  APPEND_LF();

  free(map);
  free(map_i);
  free(map_src);
}

/*******************************************************************************
*******************************************************************************/

static char * sb_int(char * map, char * i)
{
  char * map_name = next_name(), * instruction = NULL;

  prepend_map_var(map_name);
  APPEND_INSTRUCTION_ARGS(
    "%s = cmap_list_get((CMAP_LIST *)%s, %s);", map_name, map, i);
  APPEND_LF();

  free(map);
  free(i);

  return map_name;
}

static char * sb_string(char * map, char * string)
{
  char * map_name = next_name(), * instruction = NULL;

  prepend_map_var(map_name);
  APPEND_INSTRUCTION_ARGS("%s = cmap_get(%s, %s);", map_name, map, string);
  APPEND_LF();

  free(map);
  free(string);

  return map_name;
}

static char * sb_map(char * map, char * map_i)
{
  char * map_name = next_name(), * instruction = NULL;

  prepend_map_var(map_name);
  APPEND_INSTRUCTION_ARGS("if(cmap_nature(%s) == CMAP_INT_NATURE)", map_i);
  APPEND_INSTRUCTION_ARGS(
    SPACE "%s = cmap_list_get((CMAP_LIST *)%s, cmap_int_get((CMAP_INT *)%s));",
    map_name, map, map_i);
  APPEND_INSTRUCTION_ARGS(
    "else if(cmap_nature(%s) == CMAP_STRING_NATURE)", map_i);
  APPEND_INSTRUCTION_ARGS(
    SPACE "%s = cmap_get(%s, cmap_string_val((CMAP_STRING *)%s));",
    map_name, map, map_i);
  APPEND_LF();

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
  char * call = next_name();

  function_c_to_part(cmap_part_public.functions(), strdup(call), (1 == 1));

  cmap_string_public.append(&call, "(proc_ctx)");

  return call;
}

static void for_decl(char * init_call, char * cmp_call, char * loop_call)
{
  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("for(%s; %s; %s)", init_call, cmp_call, loop_call);
  free(init_call); free(cmp_call); free(loop_call);

  APPEND_INSTRUCTION("{");
}

static void for_impl()
{
  end_of_instructions_part();
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
  name, path,
  set_local, set_global, set_path,
  args, args_push,
  arg_name,
  args_map, args_map_push,
  map_args, list_args, string, int_,
  return_,
  process_prepare, process_prepare_fn, process_resolve, process_c,
  function,
  c_impl, c_impl_root,
  if_, else_empty, else_if, else_,
  CMAP_PARSER_UTIL_CMP_LOOP(CMP_SET) cmp_unique,
  new,
  set_sb_int, set_sb_string, set_sb_map, sb_int, sb_string, sb_map,
  names,
  for_helper, for_decl, for_impl,
  or, and
};
