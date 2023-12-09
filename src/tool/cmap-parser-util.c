
#include "cmap-parser-util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-part.h"
#include "cmap-kv.h"
#include "cmap-fn-name.h"

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

#define APPEND_INSTRUCTION_AA_ARGS(txt, args_...) \
  cmap_string_public.append_args(&instruction, txt, args_); \
  add_aisle(&instruction, aisle); \
  add_args(&instruction, args); \
  cmap_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

#define APPEND_INSTRUCTION_AISLE_ARGS(txt, args...) \
  cmap_string_public.append_args(&instruction, txt, args); \
  add_aisle(&instruction, aisle); \
  cmap_string_public.append(&instruction, ");"); \
  cmap_part_public.add_instruction(instruction); \
  free(instruction); \
  instruction = NULL

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

typedef struct STACK_ELMT STACK_ELMT;

struct STACK_ELMT
{
  void * ptr;
  char c;
  STACK_ELMT * next;
};

static void push_elmt(STACK_ELMT ** elmts, void * ptr, char c)
{
  STACK_ELMT * elmt = (STACK_ELMT *)malloc(sizeof(STACK_ELMT));
  elmt -> ptr = ptr;
  elmt -> c = c;
  elmt -> next = *elmts;
  *elmts = elmt;
}

static STACK_ELMT * pop_elmt(STACK_ELMT ** elmts)
{
  STACK_ELMT * elmt = *elmts;
  *elmts = elmt -> next;
  return elmt;
}

static char * pop_elmt_char_ptr(STACK_ELMT ** elmts)
{
  STACK_ELMT * elmt = pop_elmt(elmts);
  char * ret = (char *)elmt -> ptr;
  free(elmt);
  return ret;
}

static char pop_elmt_char(STACK_ELMT ** elmts)
{
  STACK_ELMT * elmt = pop_elmt(elmts);
  char ret = elmt -> c;
  free(elmt);
  return ret;
}

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
  const char * tmp = cmap_kv_public.get(cmap_part_public.name2map(), name);
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
  cmap_kv_public.put(cmap_part_public.name2map(), name, map);

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
  const char * tmp = cmap_kv_public.get(cmap_part_public.name2map(), name);
  char * instruction = NULL;

  if(tmp != NULL)
  {
    cmap_kv_public.put(cmap_part_public.name2map(), name, map);

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
    PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
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
    PREPEND_INSTRUCTION_ARGS("CMAP_MAP * %s;", map_name);
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

static STACK_ELMT * process_way = NULL, * process_map_or_fn = NULL,
  * process_fn_name = NULL, * process_args = NULL;

static void process_prepare(char * map, char * fn_name, char * args)
{
  push_elmt(&process_way, NULL, PROCESS_WAY_DFT);
  push_elmt(&process_map_or_fn, map, 0);
  push_elmt(&process_fn_name, fn_name, 0);
  push_elmt(&process_args, args, 0);
}

static void process_prepare_fn(char * fn, char * args)
{
  push_elmt(&process_way, NULL, PROCESS_WAY_FN);
  push_elmt(&process_map_or_fn, fn, 0);
  push_elmt(&process_args, args, 0);
}

static char * process_resolve(char need_ret)
{
  if(pop_elmt_char(&process_way) == PROCESS_WAY_FN)
  {
    return process_fn(pop_elmt_char_ptr(&process_map_or_fn),
      pop_elmt_char_ptr(&process_args), need_ret);
  }
  else
  {
    return process(pop_elmt_char_ptr(&process_map_or_fn),
      pop_elmt_char_ptr(&process_fn_name), pop_elmt_char_ptr(&process_args),
      need_ret);
  }
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
    APPEND_INSTRUCTION_ARGS("%s = cmap_pop_local_ctx(proc_ctx, %s(proc_ctx));",
      map_name, fn_name);
  }
  else
  {
    APPEND_INSTRUCTION_ARGS("%s(proc_ctx);", fn_name);

    APPEND_INSTRUCTION("cmap_pop_local_ctx(proc_ctx, NULL);");
  }
  APPEND_LF();

  free(fn_name);

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

    if(!cmap_part_public.is_return()) APPEND_INSTRUCTION("return NULL;");
    cmap_part_public.pop_instructions_to_part(cmap_part_public.functions());

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

static void if_(char * bool_name)
{
  char * instructions = cmap_part_public.pop_instructions(),
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

  cmap_part_public.push_instructions();
}

static void else_if_stop()
{
  end_of_instructions_part();
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

static char * cmp_to_bool_fn(char * cmp_name, const char * op)
{
  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("return (%s %s 0);", cmp_name, op);
  free(cmp_name);

  char * bool_fn_name = next_name(),
    * instructions = cmap_part_public.pop_instructions();
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

static void for_decl(char * cmp_name)
{
  char * fn_end = next_name(), * fn_init = next_name();

  function_c_to_part(cmap_part_public.functions(), strdup(fn_end), (1 == 1));
  function_c_to_part(cmap_part_public.functions(), strdup(fn_init), (1 == 1));

  char * instruction = NULL;
  APPEND_INSTRUCTION_ARGS("for(%s(proc_ctx); %s; %s(proc_ctx))",
    fn_init, cmp_name, fn_end);
  free(cmp_name); free(fn_end); free(fn_init);

  APPEND_INSTRUCTION("{");
}

static void for_impl()
{
  end_of_instructions_part();
}

/*******************************************************************************
*******************************************************************************/

static char * or_and(char * cmp_name_l, char * cmp_name_r, const char * op)
{
  char * ret = NULL;
  cmap_string_public.append_args(&ret, "(%s) %s (%s)",
    cmp_name_l, op, cmp_name_r);
  free(cmp_name_l);
  free(cmp_name_r);
  return ret;
}

static char * or(char * cmp_name_l, char * cmp_name_r)
{
  return or_and(cmp_name_l, cmp_name_r, "||");
}

static char * and(char * cmp_name_l, char * cmp_name_r)
{
  return or_and(cmp_name_l, cmp_name_r, "&&");
}

/*******************************************************************************
*******************************************************************************/

#define CMP_SET(name, op) name,

const CMAP_PARSER_UTIL_PUBLIC cmap_parser_util_public =
{
  include_,
  function_c,
  instructions_root,
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
  process_prepare,
  process_prepare_fn,
  process_resolve,
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
  names,
  for_decl,
  for_impl,
  or,
  and
};
