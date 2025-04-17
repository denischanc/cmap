
#include "cmap-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-kv.h"
#include "cmap-option.h"
#include "cmap-stack-define.h"
#include "cmap-strings.h"

/*******************************************************************************
*******************************************************************************/

typedef struct CTX_CMAP CTX_CMAP;

struct CTX_CMAP
{
  CMAP_STRINGS * vars_loc, * vars_def;

  CTX_CMAP * prev;
};

typedef struct CTX_C CTX_C;

struct CTX_C
{
  char definitions, global_env, return_, return_fn;

  CMAP_KV * name2map;

  CTX_CMAP * cmap;

  CTX_C * prev;
};

typedef struct CTX_BLOCK CTX_BLOCK;

struct CTX_BLOCK
{
  char * instructions, * prefix, else_;

  CTX_C * c;
  CTX_BLOCK * block;
};

typedef struct
{
  CTX_CMAP cmap;
  CTX_C c;
  CTX_BLOCK block;
} CTX;

CMAP_STACK_DEF(ctx, CTX)

/*******************************************************************************
*******************************************************************************/

const char CMAP_PART_CTX_NATURE_DFT = 0, CMAP_PART_CTX_NATURE_FN = 1,
  CMAP_PART_CTX_NATURE_ROOT = 2;

#define PART_VAR(name) static char * name = NULL;

CMAP_PART_LOOP(PART_VAR)

static CMAP_STACK_ctx * ctxs = NULL;

static char is_new_ctx, ctx_nature;

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

static CTX ctx_new()
{
  CTX ctx;

  ctx.block.instructions = strdup("");
  ctx.block.prefix = strdup(SPACE);
  ctx.block.else_ = (1 == 0);

  ctx.c.definitions = (1 == 0);
  ctx.c.global_env = (1 == 0);
  ctx.c.return_ = (1 == 0);
  ctx.c.name2map = NULL;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CTX ctx_root_fn(char return_fn, CTX_CMAP * cmap_prev)
{
  CTX ctx = ctx_new();

  ctx.c.return_fn = return_fn;
  ctx.c.prev = NULL;

  ctx.cmap.vars_loc = NULL;
  ctx.cmap.vars_def = NULL;
  ctx.cmap.prev = cmap_prev;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CTX ctx_root()
{
  return ctx_root_fn((1 == 0), NULL);
}

static CTX ctx_fn(CTX_C * c_cur)
{
  return ctx_root_fn((1 == 1), c_cur -> cmap);
}

static CTX ctx_dft(CTX_C * c_cur)
{
  CTX ctx = ctx_new();

  ctx.c.return_fn = (1 == 0);
  ctx.c.cmap = c_cur -> cmap;
  ctx.c.prev = c_cur;

  return ctx;
}

static CTX ctx_block(CTX_C * c_cur)
{
  CTX ctx;

  ctx.block.instructions = strdup("");
  ctx.block.prefix = strdup(ctxs -> v.block.prefix);
  cmap_string_public.append(&ctx.block.prefix, SPACE);
  ctx.block.else_ = (1 == 0);
  ctx.block.c = c_cur;
  ctx.block.block = ctxs -> v.block.block;

  ctx.c.name2map = NULL;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CTX get_ctx(CTX_C * c_cur)
{
  if(is_new_ctx)
  {
    if(ctx_nature == CMAP_PART_CTX_NATURE_ROOT) return ctx_root();
    else if(ctx_nature == CMAP_PART_CTX_NATURE_FN) return ctx_fn(c_cur);
    else return ctx_dft(c_cur);
  }
  else return ctx_block(c_cur);
}

/*******************************************************************************
*******************************************************************************/

static void push_instructions()
{
  CTX_C * c_cur = NULL;
  if(ctxs == NULL) new_ctx(CMAP_PART_CTX_NATURE_ROOT);
  else c_cur = ctxs -> v.block.c;

  cmap_stack_ctx_push(&ctxs, get_ctx(c_cur));

  if(is_new_ctx)
  {
    CTX * ctx = &ctxs -> v;

    ctx -> block.c = &ctx -> c;
    ctx -> block.block = &ctx -> block;
    if((ctx_nature == CMAP_PART_CTX_NATURE_FN) ||
      (ctx_nature == CMAP_PART_CTX_NATURE_ROOT))
      ctx -> c.cmap = &ctx -> cmap;

    is_new_ctx = (1 == 0);
  }
}

/*******************************************************************************
*******************************************************************************/

static char ** instructions_()
{
  return &ctxs -> v.block.instructions;
}

/*******************************************************************************
*******************************************************************************/

static void add_instruction(const char * instruction)
{
  cmap_string_public.append_args(instructions_(), "%s%s\n",
    ctxs -> v.block.prefix, instruction);
}

/*******************************************************************************
*******************************************************************************/

static void add_lf()
{
  cmap_string_public.append(instructions_(), "\n");
}

/*******************************************************************************
*******************************************************************************/

static void prepend_instruction(const char * instruction)
{
  char ** instructions = &ctxs -> v.block.block -> instructions;

  char * tmp = *instructions;
  *instructions = NULL;
  cmap_string_public.append_args(instructions, SPACE "%s\n\n", instruction);
  cmap_string_public.append(instructions, tmp);
  free(tmp);
}

/*******************************************************************************
*******************************************************************************/

static char is_definitions()
{
  CTX_C * ctx = ctxs -> v.block.c;
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
  CTX_C * ctx = ctxs -> v.block.c;
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
  return &ctxs -> v.block.c -> name2map;
}

/*******************************************************************************
*******************************************************************************/

static char * pop_instructions()
{
  CTX ctx = cmap_stack_ctx_pop(&ctxs);
  cmap_kv_public.delete(&ctx.c.name2map);
  free(ctx.block.prefix);
  return ctx.block.instructions;
}

static void pop_instructions_to_part(char ** part)
{
  char * instructions_to_append = pop_instructions();
  if(part == NULL) part = instructions_();
  cmap_string_public.append(part, instructions_to_append);
  free(instructions_to_append);
}

/*******************************************************************************
*******************************************************************************/

static void return_()
{
  ctxs -> v.block.c -> return_ = (1 == 1);
}

static char is_return()
{
  return ctxs -> v.block.c -> return_;
}

/*******************************************************************************
*******************************************************************************/

static void return_fn()
{
  ctxs -> v.block.c -> return_fn = (1 == 1);
}

static char is_return_fn()
{
  return ctxs -> v.block.c -> return_fn;
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
  ctxs -> v.block.else_ = (1 == 1);
}

static char is_n_rst_else()
{
  char ret = ctxs -> v.block.else_;
  ctxs -> v.block.else_ = (1 == 0);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

#define PART_FREE(name) free(name); name = NULL;

static void clean()
{
  CMAP_PART_LOOP(PART_FREE)

  new_ctx(CMAP_PART_CTX_NATURE_ROOT);
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
