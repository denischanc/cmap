
#include "cmap-parser-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-parser-part.h"
#include "cmap-kv.h"

/*******************************************************************************
*******************************************************************************/

#define APPEND_ARGS(part, txt, args...) \
  cmap_string_public.append_args(cmap_parser_part_public.part(), txt, args)

#define APPEND(part, txt) \
  cmap_string_public.append(cmap_parser_part_public.part(), txt)

#define APPEND_INSTRUCTION_ARGS(txt, args...) \
  cmap_string_public.append_args(&instruction, txt, args); \
  cmap_parser_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

#define APPEND_INSTRUCTION(txt) cmap_parser_part_public.add_instruction(txt)

#define APPEND_LF() cmap_parser_part_public.add_lf()

#define APPEND_INSTRUCTION_AA_ARGS(txt, args_...) \
  cmap_string_public.append_args(&instruction, txt, args_); \
  add_aisle(&instruction, aisle); \
  add_args(&instruction, args); \
  cmap_parser_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

#define APPEND_INSTRUCTION_AISLE_ARGS(txt, args...) \
  cmap_string_public.append_args(&instruction, txt, args); \
  add_aisle(&instruction, aisle); \
  cmap_string_public.append(&instruction, ");"); \
  cmap_parser_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

#define APPEND_INSTRUCTION_ARGS_ARGS(txt, args_...) \
  cmap_string_public.append_args(&instruction, txt, args_); \
  add_args(&instruction, args); \
  cmap_parser_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

#define PREPEND_INSTRUCTION_ARGS(txt, args...) \
  cmap_string_public.append_args(&instruction, txt, args); \
  cmap_parser_part_public.prepend_instruction(instruction); \
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
  static char buffer[30];
  snprintf(buffer, sizeof(buffer), "cmap_gen_name__%d", id++);
  return strdup(buffer);
}

/*******************************************************************************
*******************************************************************************/

static const char * add_definitions()
{
  if(!cmap_parser_part_public.is_definitions())
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
  if(!cmap_parser_part_public.is_global_env())
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
    cmap_string_public.append_args(instruction, ", %s", args);
    free(args);
  }

  cmap_string_public.append(instruction, ", NULL);");
}

static void add_aisle(char ** instruction, char * aisle)
{
  if(aisle != NULL)
  {
    if(!strcmp(aisle, "\"local\""))
      cmap_string_public.append(instruction, "CMAP_AISLE_LOCAL");
    else if(!strcmp(aisle, "\"global\""))
      cmap_string_public.append(instruction, "CMAP_AISLE_GLOBAL");
    else cmap_string_public.append(instruction, aisle);
    free(aisle);
  }
  else cmap_string_public.append(instruction, "NULL");
}

/*******************************************************************************
*******************************************************************************/

static void function_c(char * name, char is_static)
{
  char is_return_fn = cmap_parser_part_public.is_return_fn();
  APPEND_ARGS(main, "%s%s %s(CMAP_PROC_CTX * proc_ctx)\n{\n",
    is_static ? "static " : "", is_return_fn ? "CMAP_MAP *" : "void", name);

  if(!is_static)
  {
    APPEND_ARGS(headers, "%s %s(CMAP_PROC_CTX * proc_ctx);\n",
      is_return_fn ? "CMAP_MAP *" : "void", name);
  }

  free(name);

  if(is_return_fn && !cmap_parser_part_public.is_return())
    APPEND_INSTRUCTION("return NULL;");
  char * instructions = cmap_parser_part_public.pop_instructions();
  APPEND(main, instructions);
  free(instructions);

  APPEND(main, "}\n\n");
}

/*******************************************************************************
*******************************************************************************/

static char * name(char * name)
{
  const char * tmp = cmap_kv_public.get(
    cmap_parser_part_public.name2map(), name);
  if(tmp != NULL) return strdup(tmp);
  else
  {
    char * map_name = next_name(), * instruction = NULL;

    PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
    APPEND_INSTRUCTION_ARGS("if((%s != NULL) && cmap_is_key(%s, \"%s\"))",
      add_definitions(), add_definitions(), name);
    APPEND_INSTRUCTION_ARGS(SPACE "%s = cmap_get(%s, \"%s\");",
      map_name, add_definitions(), name);
    APPEND_INSTRUCTION_ARGS("else %s = cmap_get(%s, \"%s\");",
      map_name, add_global_env(), name);
    APPEND_LF();

    free(name);

    return map_name;
  }
}

/*******************************************************************************
*******************************************************************************/

static char * path(char * map, char * name)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
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
  cmap_kv_public.put(cmap_parser_part_public.name2map(), name, map);

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
  const char * tmp = cmap_kv_public.get(
    cmap_parser_part_public.name2map(), name);
  char * instruction = NULL;

  if(tmp != NULL)
  {
    cmap_kv_public.put(cmap_parser_part_public.name2map(), name, map);

    APPEND_INSTRUCTION_ARGS("cmap_set(%s, \"%s\", %s);",
      add_definitions(), name, map);
  }
  else
  {
    APPEND_INSTRUCTION_ARGS("if((%s != NULL) && cmap_is_key(%s, \"%s\"))",
      add_definitions(), add_definitions(), name);
    APPEND_INSTRUCTION_ARGS(SPACE "cmap_set(%s, \"%s\", %s);",
      add_definitions(), name, map);
    APPEND_INSTRUCTION_ARGS("else cmap_set(%s, \"%s\", %s);",
      add_global_env(), name, map);
  }
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

static char * arg_names(char * name)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "\"%s\"", name);

  free(name);

  return ret;
}

static char * arg_names_push(char * list, char * name)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "%s, \"%s\"", list, name);

  free(list);
  free(name);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static char * args_map(char * name, char * map)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "\"%s\", %s", name, map);

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

static char * map_args(char * args, char * aisle)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AA_ARGS("%s = cmap_to_map(proc_ctx, ", map_name);
  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * list_args(char * args, char * aisle)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AA_ARGS(
    "%s = (CMAP_MAP *)cmap_to_list(proc_ctx, ", map_name);
  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * string(char * string, char * aisle)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AISLE_ARGS(
    "%s = (CMAP_MAP *)cmap_string(%s, 0, proc_ctx, ", map_name, string);
  APPEND_LF();

  free(string);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * int_(char * i, char * aisle)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AISLE_ARGS(
    "%s = (CMAP_MAP *)cmap_int(%s, proc_ctx, ", map_name, i);
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
    if(cmap_parser_part_public.is_return_fn())
      APPEND_INSTRUCTION("return NULL;");
    else APPEND_INSTRUCTION("return;");
  }
  else
  {
    char * instruction = NULL;
    APPEND_INSTRUCTION_ARGS("return %s;", map);
    free(map);

    cmap_parser_part_public.return_fn();
    cmap_parser_part_public.return_();
  }
}

/*******************************************************************************
*******************************************************************************/

static char * process(char * map, char * fn_name, char * args)
{
  char * map_keep = (map == NULL) ? strdup("NULL") : strdup(map),
    * map_fn = (map == NULL) ? name(fn_name) : path(map, fn_name),
    * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_ARGS_ARGS(
    "%s = cmap_fn_proc((CMAP_FN *)%s, proc_ctx, %s",
    map_name, map_fn, map_keep);
  APPEND_LF();

  free(map_fn);
  free(map_keep);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * process_fn(char * fn, char * args)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_ARGS_ARGS(
    "%s = cmap_fn_proc((CMAP_FN *)%s, proc_ctx, NULL", map_name, fn);
  APPEND_LF();

  free(fn);

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * process_c(char * fn_name, char need_ret)
{
  char * instruction = NULL, * map_name = NULL;

  APPEND_INSTRUCTION("cmap_push_local_ctx(proc_ctx);");

  if(need_ret)
  {
    map_name = next_name();
    PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
    APPEND_INSTRUCTION_ARGS("%s = %s(proc_ctx);", map_name, fn_name);
  }
  else
  {
    APPEND_INSTRUCTION_ARGS("%s(proc_ctx);", fn_name);
  }

  free(fn_name);

  APPEND_INSTRUCTION("cmap_pop_local_ctx(proc_ctx);");
  APPEND_LF();

  return map_name;
}

/*******************************************************************************
*******************************************************************************/

static char * function(char * args, char * aisle, char * fn_name)
{
  char * map_name = next_name(), * instruction = NULL;

  if(fn_name == NULL)
  {
    fn_name = next_name();

    APPEND_ARGS(functions,
      "static CMAP_MAP * %s(CMAP_PROC_CTX * proc_ctx,\n"
      "  CMAP_MAP * map, CMAP_LIST * args)\n{\n", fn_name);

    if(!cmap_parser_part_public.is_return())
      APPEND_INSTRUCTION("return NULL;");
    char * instructions = cmap_parser_part_public.pop_instructions();
    APPEND(functions, instructions);
    free(instructions);

    APPEND(functions, "}\n\n");
  }

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AISLE_ARGS(
    "%s = (CMAP_MAP *)cmap_fn(%s, proc_ctx, ", map_name, fn_name);

  free(fn_name);

  APPEND_INSTRUCTION_ARGS(
    "cmap_copy_map(cmap_fn_require_definitions((CMAP_FN *)%s, proc_ctx), %s);",
    map_name, add_definitions());

  if(args != NULL)
  {
    APPEND_INSTRUCTION_ARGS_ARGS(
      "cmap_add_arg_names((CMAP_FN *)%s, proc_ctx", map_name);
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

static void if_(char * bool_name)
{
  char * instructions = cmap_parser_part_public.pop_instructions(),
    * instruction = NULL;

  APPEND_INSTRUCTION_ARGS("if(%s)", bool_name);
  free(bool_name);
  APPEND_INSTRUCTION("{");
  APPEND(instructions, instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
}

static void else_empty()
{
  APPEND_LF();
}

static void else_if_start()
{
  APPEND_INSTRUCTION("else");
  APPEND_INSTRUCTION("{");

  cmap_parser_part_public.push_instructions();
}

static void else_if_stop()
{
  char * instructions = cmap_parser_part_public.pop_instructions();
  APPEND(instructions, instructions);
  free(instructions);

  APPEND_INSTRUCTION("}");
  APPEND_LF();
}

static void else_()
{
  char * instructions = cmap_parser_part_public.pop_instructions();

  APPEND_INSTRUCTION("else");
  APPEND_INSTRUCTION("{");
  APPEND(instructions, instructions);
  free(instructions);
  APPEND_INSTRUCTION("}");
  APPEND_LF();
}

/*******************************************************************************
*******************************************************************************/

static char * cmp_to_bool_fn(char * cmp_name, const char * op)
{
  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("return (%s %s 0);", cmp_name, op);
  free(cmp_name);

  char * bool_fn_name = next_name(),
    * instructions = cmap_parser_part_public.pop_instructions();
  APPEND_ARGS(functions,
    "static inline char %s(CMAP_PROC_CTX * proc_ctx)\n{\n", bool_fn_name);
  APPEND(functions, instructions);
  free(instructions);
  APPEND(functions, "}\n\n");

  cmap_string_public.append(&bool_fn_name, "(proc_ctx)");
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

static char * new(char * map, char * args, char * aisle)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_AA_ARGS(
    "%s = cmap_new((CMAP_FN *)%s, proc_ctx, ", map_name, map);
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

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
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

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
  APPEND_INSTRUCTION_ARGS("%s = cmap_get(%s, %s);", map_name, map, string);
  APPEND_LF();

  free(map);
  free(string);

  return map_name;
}

static char * sb_map(char * map, char * map_i)
{
  char * map_name = next_name(), * instruction = NULL;

  PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s = NULL;", map_name);
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

static char * aisle_names(char * names)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "\"%s\"", names);
  free(names);
  return ret;
}

static char * aisle_map(char * map)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "cmap_string_val((CMAP_STRING *)%s)",
    map);
  free(map);
  return ret;
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

#define CMP_SET(name, op) name,

const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public =
{
  function_c,
  name,
  path,
  set_local,
  set_global,
  set_path,
  args_push,
  arg_names,
  arg_names_push,
  args_map,
  args_map_push,
  map_args,
  list_args,
  string,
  int_,
  return_,
  process,
  process_fn,
  process_c,
  function,
  c_impl,
  c_impl_root,
  if_,
  else_empty,
  else_if_start,
  else_if_stop,
  else_,
  CMAP_PARSER_UTIL_CMP_LOOP(CMP_SET)
  cmp_unique,
  new,
  set_sb_int,
  set_sb_string,
  set_sb_map,
  sb_int,
  sb_string,
  sb_map,
  aisle_names,
  aisle_map,
  names
};
