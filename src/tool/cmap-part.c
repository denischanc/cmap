
#include "cmap-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-kv.h"
#include "cmap-option.h"

/*******************************************************************************
*******************************************************************************/

typedef struct INSTRUCTIONS INSTRUCTIONS;

struct INSTRUCTIONS
{
  char * instructions, definitions, global_env, return_, return_fn, * prefix;

  CMAP_KV * name2map;

  INSTRUCTIONS * next, * ctx;
};

/*******************************************************************************
*******************************************************************************/

const char CMAP_PART_CTX_NATURE_DFT = 0, CMAP_PART_CTX_NATURE_FN = 1;

#define PART_VAR(name) static char * name = NULL;

CMAP_PART_LOOP(PART_VAR)

static INSTRUCTIONS * instructions = NULL;

static char is_new_ctx = (1 == 1), ctx_nature = CMAP_PART_CTX_NATURE_DFT;

/*******************************************************************************
*******************************************************************************/

#define PART_FN(name) \
static char ** name##_() \
{ \
  if(name == NULL) name = strdup(""); \
  return &name; \
}

CMAP_PART_LOOP(PART_FN)

/*******************************************************************************
*******************************************************************************/

static void new_ctx(char nature)
{
  is_new_ctx = (1 == 1);
  ctx_nature = nature;
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
    if(ctx_nature == CMAP_PART_CTX_NATURE_FN) tmp -> return_fn = (1 == 1);
    else tmp -> return_fn = (1 == 0);
    tmp -> prefix = strdup(SPACE);
    tmp -> ctx = tmp;
    tmp -> name2map = cmap_kv_public.create();
  }
  else
  {
    tmp -> return_fn = (1 == 0);
    tmp -> prefix = strdup(instructions -> prefix);
    cmap_string_public.append(&tmp -> prefix, SPACE);
    tmp -> ctx = instructions -> ctx;
    tmp -> name2map = NULL;
  }
  tmp -> next = instructions;
  instructions = tmp;

  is_new_ctx = (1 == 0);
  ctx_nature = CMAP_PART_CTX_NATURE_DFT;
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

/*******************************************************************************
*******************************************************************************/

static char is_definitions()
{
  INSTRUCTIONS * ctx = instructions -> ctx;
  if(ctx -> definitions) return (1 == 1);
  else
  {
    ctx -> definitions = (1 == 1);
    return (1 == 0);
  }
}

/*******************************************************************************
*******************************************************************************/

static char is_global_env()
{
  INSTRUCTIONS * ctx = instructions -> ctx;
  if(ctx -> global_env) return (1 == 1);
  else
  {
    ctx -> global_env = (1 == 1);
    return (1 == 0);
  }
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
  instructions -> ctx -> return_ = (1 == 1);
}

static char is_return()
{
  return instructions -> ctx -> return_;
}

/*******************************************************************************
*******************************************************************************/

static void return_fn()
{
  instructions -> ctx -> return_fn = (1 == 1);
}

static char is_return_fn()
{
  return instructions -> ctx -> return_fn;
}

/*******************************************************************************
*******************************************************************************/

static void add_include(const char * name)
{
  if(!strncmp(name, "cmap", 4))
  {
    cmap_string_public.append_args(&includes,
      (cmap_option_public.is_relative_inc()) ?
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

#define PART_FREE(name) free(name); name = NULL;

static void clean()
{
  CMAP_PART_LOOP(PART_FREE)

  is_new_ctx = (1 == 1);
  ctx_nature = CMAP_PART_CTX_NATURE_DFT;
}

/*******************************************************************************
*******************************************************************************/

#define PART_SET(name) name##_,

const CMAP_PART_PUBLIC cmap_part_public =
{
  clean,
  CMAP_PART_LOOP(PART_SET)
  new_ctx,
  push_instructions,
  instructions_,
  add_instruction,
  add_lf,
  prepend_instruction,
  is_definitions,
  is_global_env,
  name2map,
  pop_instructions,
  return_,
  is_return,
  return_fn,
  is_return_fn,
  add_include,
  add_relative_include,
  add_include_lf
};
