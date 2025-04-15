
#include "cmap-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-kv.h"
#include "cmap-option.h"
#include "cmap-stack-define.h"

/*******************************************************************************
*******************************************************************************/

typedef struct INSTRUCTIONS INSTRUCTIONS;

struct INSTRUCTIONS
{
  char * instructions, definitions, global_env, return_, return_fn, * prefix,
    else_;

  CMAP_KV * name2map;

  INSTRUCTIONS * ctx;
};

CMAP_STACK_DEF(instructions, INSTRUCTIONS)

/*******************************************************************************
*******************************************************************************/

const char CMAP_PART_CTX_NATURE_DFT = 0, CMAP_PART_CTX_NATURE_FN = 1;

#define PART_VAR(name) static char * name = NULL;

CMAP_PART_LOOP(PART_VAR)

static CMAP_STACK_instructions * instructions = NULL;

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
  INSTRUCTIONS tmp;
  tmp.instructions = strdup("");
  tmp.definitions = (1 == 0);
  tmp.global_env = (1 == 0);
  tmp.return_ = (1 == 0);
  tmp.else_ = (1 == 0);
  tmp.name2map = NULL;
  if(is_new_ctx || (instructions == NULL))
  {
    if(ctx_nature == CMAP_PART_CTX_NATURE_FN) tmp.return_fn = (1 == 1);
    else tmp.return_fn = (1 == 0);
    tmp.prefix = strdup(SPACE);
    tmp.ctx = NULL;
  }
  else
  {
    tmp.return_fn = (1 == 0);
    tmp.prefix = strdup(instructions -> v.prefix);
    cmap_string_public.append(&tmp.prefix, SPACE);
    tmp.ctx = instructions -> v.ctx;
  }

  cmap_stack_instructions_push(&instructions, tmp);
  if(tmp.ctx == NULL) instructions -> v.ctx = &instructions -> v;

  is_new_ctx = (1 == 0);
  ctx_nature = CMAP_PART_CTX_NATURE_DFT;
}

/*******************************************************************************
*******************************************************************************/

static char ** instructions_()
{
  return &instructions -> v.instructions;
}

/*******************************************************************************
*******************************************************************************/

static void add_instruction(const char * instruction)
{
  cmap_string_public.append_args(&instructions -> v.instructions, "%s%s\n",
    instructions -> v.prefix, instruction);
}

/*******************************************************************************
*******************************************************************************/

static void add_lf()
{
  cmap_string_public.append(&instructions -> v.instructions, "\n");
}

/*******************************************************************************
*******************************************************************************/

static void prepend_instruction(const char * instruction)
{
  INSTRUCTIONS * ctx = instructions -> v.ctx;

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
  INSTRUCTIONS * ctx = instructions -> v.ctx;
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
  INSTRUCTIONS * ctx = instructions -> v.ctx;
  if(ctx -> global_env) return (1 == 1);
  else
  {
    ctx -> global_env = (1 == 1);
    return (1 == 0);
  }
}

/*******************************************************************************
*******************************************************************************/

static CMAP_KV ** name2map_ptr()
{
  return &(instructions -> v.ctx -> name2map);
}

/*******************************************************************************
*******************************************************************************/

static char * pop_instructions()
{
  INSTRUCTIONS tmp = cmap_stack_instructions_pop(&instructions);
  cmap_kv_public.delete(&tmp.name2map);
  free(tmp.prefix);
  return tmp.instructions;
}

static void pop_instructions_to_part(char ** part)
{
  char * instructions_to_append = pop_instructions();
  if(part == NULL) part = &instructions -> v.instructions;
  cmap_string_public.append(part, instructions_to_append);
  free(instructions_to_append);
}

/*******************************************************************************
*******************************************************************************/

static void return_()
{
  instructions -> v.ctx -> return_ = (1 == 1);
}

static char is_return()
{
  return instructions -> v.ctx -> return_;
}

/*******************************************************************************
*******************************************************************************/

static void return_fn()
{
  instructions -> v.ctx -> return_fn = (1 == 1);
}

static char is_return_fn()
{
  return instructions -> v.ctx -> return_fn;
}

/*******************************************************************************
*******************************************************************************/

static void add_include(const char * name, char is_relative)
{
  if(!strncmp(name, "cmap", 4) || is_relative)
  {
    is_relative = is_relative || cmap_option_public.is_relative_inc();
    cmap_string_public.append_args(&includes,
      (is_relative) ? "#include \"%s\"\n" : "#include <cmap/%s>\n", name);
  }
  else cmap_string_public.append_args(&includes, "#include <%s>\n", name);
}

/*******************************************************************************
*******************************************************************************/

static void set_else()
{
  instructions -> v.else_ = (1 == 1);
}

static char is_n_rst_else()
{
  char ret = instructions -> v.else_;
  instructions -> v.else_ = (1 == 0);
  return ret;
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
  name2map_ptr,
  pop_instructions,
  pop_instructions_to_part,
  return_,
  is_return,
  return_fn,
  is_return_fn,
  add_include,
  set_else,
  is_n_rst_else
};
