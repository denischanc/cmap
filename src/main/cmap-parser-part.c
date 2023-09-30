
#include "cmap-parser-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-kv.h"
#include "cmap-gen.h"

/*******************************************************************************
*******************************************************************************/

#define PART_LOOP(macro) \
  macro(c_impl) \
  macro(functions) \
  macro(main) \
  macro(headers)

/*******************************************************************************
*******************************************************************************/

typedef struct INSTRUCTIONS INSTRUCTIONS;

struct INSTRUCTIONS
{
  char * instructions, definitions, global_env, return_, * prefix;

  CMAP_KV * name2map;

  INSTRUCTIONS * next, * ctx;
};

/*******************************************************************************
*******************************************************************************/

#define PART_VAR(name) static char * name = NULL;

PART_LOOP(PART_VAR)

static INSTRUCTIONS * instructions = NULL;

static char * includes = NULL, is_new_ctx = (1 == 1);

/*******************************************************************************
*******************************************************************************/

#define PART_FN(name) \
static char ** name##_() \
{ \
  if(name == NULL) name = strdup(""); \
  return &name; \
}

PART_LOOP(PART_FN)

/*******************************************************************************
*******************************************************************************/

static void new_ctx()
{
  is_new_ctx = (1 == 1);
}

/*******************************************************************************
*******************************************************************************/

static void push_instructions()
{
  INSTRUCTIONS * tmp = (INSTRUCTIONS *)malloc(sizeof(INSTRUCTIONS));
  tmp -> instructions = strdup("");
  tmp -> definitions = (1 == 0);
  tmp -> global_env = (1 == 0);
  tmp -> return_ = (1 == 0);
  if(is_new_ctx || (instructions == NULL))
  {
    is_new_ctx = (1 == 0);
    tmp -> prefix = strdup(SPACE);
    tmp -> ctx = tmp;
    tmp -> name2map = cmap_kv_public.create();
  }
  else
  {
    tmp -> prefix = strdup(instructions -> prefix);
    cmap_string_public.append(&tmp -> prefix, SPACE);
    tmp -> ctx = instructions -> ctx;
    tmp -> name2map = NULL;
  }
  tmp -> next = instructions;
  instructions = tmp;
}

/*******************************************************************************
*******************************************************************************/

static char ** instructions_()
{
  return &instructions -> instructions;
}

/*******************************************************************************
*******************************************************************************/

static void add_instruction(const char * instruction)
{
  cmap_string_public.append_args(&instructions -> instructions, "%s%s\n",
    instructions -> prefix, instruction);
}

/*******************************************************************************
*******************************************************************************/

static void add_lf()
{
  cmap_string_public.append(&instructions -> instructions, "\n");
}

/*******************************************************************************
*******************************************************************************/

static void prepend_instruction(const char * instruction)
{
  INSTRUCTIONS * ctx = instructions -> ctx;

  char * tmp = ctx -> instructions;
  ctx -> instructions = NULL;
  cmap_string_public.append_args(&ctx -> instructions,
    SPACE "%s\n\n", instruction);
  cmap_string_public.append(&ctx -> instructions, tmp);
  free(tmp);
}

static void add_definitions(const char * instruction)
{
  INSTRUCTIONS * ctx = instructions -> ctx;

  if(!ctx -> definitions)
  {
    ctx -> definitions = (1 == 1);
    prepend_instruction(instruction);
  }
}

static void add_global_env(const char * instruction)
{
  INSTRUCTIONS * ctx = instructions -> ctx;

  if(!ctx -> global_env)
  {
    ctx -> global_env = (1 == 1);
    prepend_instruction(instruction);
  }
}

/*******************************************************************************
*******************************************************************************/

static void add_prefix()
{
  cmap_string_public.append(&instructions -> instructions,
    instructions -> prefix);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_KV * name2map()
{
  return instructions -> ctx -> name2map;
}

/*******************************************************************************
*******************************************************************************/

static char * pop_instructions()
{
  char * ret = instructions -> instructions;
  INSTRUCTIONS * tmp = instructions;
  instructions = instructions -> next;
  if(tmp -> name2map != NULL) cmap_kv_public.delete(tmp -> name2map);
  free(tmp -> prefix);
  free(tmp);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void return_()
{
  instructions -> return_ = (1 == 1);
}

/*******************************************************************************
*******************************************************************************/

static char is_return()
{
  return instructions -> return_;
}

/*******************************************************************************
*******************************************************************************/

static void add_include(const char * name)
{
  if(!strncmp(name, "cmap", 4))
  {
    cmap_string_public.append_args(&includes,
      (cmap_gen_public.relative_inc()) ?
        "#include \"%s\"\n" : "#include <cmap/%s>\n", name);
  }
  else cmap_string_public.append_args(&includes, "#include <%s>\n", name);
}

/*******************************************************************************
*******************************************************************************/

static void add_relative_include(const char * name)
{
  cmap_string_public.append_args(&includes, "#include \"%s\"\n", name);
}

/*******************************************************************************
*******************************************************************************/

static void add_include_lf()
{
  cmap_string_public.append(&includes, "\n");
}

/*******************************************************************************
*******************************************************************************/

static char * includes_()
{
  return includes;
}

/*******************************************************************************
*******************************************************************************/

#define PART_DECL(name) name##_,

const CMAP_PARSER_PART_PUBLIC cmap_parser_part_public =
{
  PART_LOOP(PART_DECL)
  new_ctx,
  push_instructions,
  instructions_,
  add_instruction,
  add_lf,
  prepend_instruction,
  add_definitions,
  add_global_env,
  add_prefix,
  name2map,
  pop_instructions,
  return_,
  is_return,
  add_include,
  add_relative_include,
  add_include_lf,
  includes_
};
