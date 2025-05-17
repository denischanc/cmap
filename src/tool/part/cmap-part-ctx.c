
#include "cmap-part-ctx.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-stack-define.h"
#include "cmap-part-var.h"
#include "cmap-part-this-args.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_STRINGS * vars_loc, * vars_def;

  CMAP_PART_CTX * prev;
} CTX_CMAP;

typedef struct
{
  char definitions, global_env, return_, return_fn;

  CMAP_PART_KV * name2map;
  CMAP_STRINGS * params;

  CMAP_PART_CTX * cmap, * prev;
} CTX_C;

typedef struct
{
  char * instructions, * prefix, else_, nature;

  CMAP_STRINGS * fn_arg_names;

  CMAP_PART_KEYS * affecteds;

  CMAP_PART_CTX * c, * next;
} CTX_BLOCK;

struct CMAP_PART_CTX
{
  CTX_CMAP cmap;
  CTX_C c;
  CTX_BLOCK block;
};

CMAP_STACK_DEF(CTX, ctx, CMAP_PART_CTX)

/*******************************************************************************
*******************************************************************************/

static const char CMAP_PART_CTX_NATURE_FN = 0, CMAP_PART_CTX_NATURE_ROOT = 1,
  CMAP_PART_CTX_NATURE_BLOCK = 2, CMAP_PART_CTX_NATURE_PARAMS = 3,
  CMAP_PART_CTX_NATURE_PROC = 4;

static char nature = CMAP_PART_CTX_NATURE_BLOCK;

static CMAP_STACK_CTX * ctxs = NULL;

/*******************************************************************************
*******************************************************************************/

#define NATURE_IMPL(NAME, name) \
static void nature_##name() \
{ \
  nature = CMAP_PART_CTX_NATURE_##NAME; \
}

CMAP_PART_CTX_NATURE_LOOP(NATURE_IMPL)

/*******************************************************************************
*******************************************************************************/

static char is_cmap(CMAP_PART_CTX * ctx)
{
  char nature = ctx -> block.nature;
  return ((nature == CMAP_PART_CTX_NATURE_ROOT) ||
    (nature == CMAP_PART_CTX_NATURE_FN));
}

static char is_c(CMAP_PART_CTX * ctx)
{
  char nature = ctx -> block.nature;
  return (nature != CMAP_PART_CTX_NATURE_BLOCK);
}

static char is_params(CMAP_PART_CTX * ctx)
{
  char nature = ctx -> block.nature;
  return (nature == CMAP_PART_CTX_NATURE_PARAMS);
}

static char is_fn(CMAP_PART_CTX * ctx)
{
  char nature = ctx -> block.nature;
  return (nature == CMAP_PART_CTX_NATURE_FN);
}

static char is_root(CMAP_PART_CTX * ctx)
{
  char nature = ctx -> block.nature;
  return (nature == CMAP_PART_CTX_NATURE_ROOT);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX ctx_common()
{
  CMAP_PART_CTX ctx;

  ctx.block.instructions = strdup("");
  ctx.block.else_ = (1 == 0);
  ctx.block.fn_arg_names = NULL;
  ctx.block.affecteds = NULL;
  ctx.block.next = NULL;

  ctx.c.name2map = NULL;
  ctx.c.params = NULL;

  ctx.cmap.vars_loc = NULL;
  ctx.cmap.vars_def = NULL;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX ctx_new()
{
  CMAP_PART_CTX ctx = ctx_common();

  ctx.block.prefix = strdup(SPACE);

  ctx.c.definitions = (1 == 0);
  ctx.c.global_env = (1 == 0);
  ctx.c.return_ = (1 == 0);

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX ctx_root_fn(char return_fn, CMAP_PART_CTX * prev)
{
  CMAP_PART_CTX ctx = ctx_new();

  ctx.c.return_fn = return_fn;
  ctx.c.prev = NULL;

  ctx.cmap.prev = prev;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX ctx_root()
{
  CMAP_PART_CTX ctx = ctx_root_fn((1 == 0), NULL);
  ctx.block.nature = CMAP_PART_CTX_NATURE_ROOT;
  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX ctx_fn(CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx = ctx_root_fn((1 == 1), block_ -> c -> c.cmap);
  ctx.block.nature = CMAP_PART_CTX_NATURE_FN;

  cmap_strings_public.add_all(&ctx.cmap.vars_loc, block_ -> fn_arg_names);

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX ctx_params(CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx = ctx_new();
  ctx.block.nature = CMAP_PART_CTX_NATURE_PARAMS;

  ctx.c.return_fn = (1 == 0);
  CMAP_PART_CTX * c = block_ -> c;
  ctx.c.cmap = c -> c.cmap;
  ctx.c.prev = c;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX ctx_proc(CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx = ctx_params(block_);
  ctx.block.nature = CMAP_PART_CTX_NATURE_PROC;
  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX ctx_block(CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx = ctx_common();
  ctx.block.nature = CMAP_PART_CTX_NATURE_BLOCK;

  ctx.block.prefix = strdup(block_ -> prefix);
  cmap_string_public.append(&ctx.block.prefix, SPACE);
  cmap_part_keys_public.add_all(&ctx.block.affecteds, block_ -> affecteds);
  ctx.block.c = block_ -> c;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX * cur_ctx()
{
  return &ctxs -> v;
}

/*******************************************************************************
*******************************************************************************/

static void upd(CTX_BLOCK * block_)
{
  CMAP_PART_CTX * ctx = cur_ctx();
  if(is_c(ctx))
  {
    ctx -> block.c = ctx;
    if(is_cmap(ctx))
    {
      ctx -> c.cmap = ctx;
      if(is_fn(ctx)) cmap_part_this_args_public.add();
    }
  }
  else
  {
    block_ -> next = ctx;
  }
}

static void push()
{
  CTX_BLOCK * block_ = &cur_ctx() -> block;

  CMAP_PART_CTX ctx;
  if(ctxs == NULL) ctx = ctx_root();
  else
  {
    if(nature == CMAP_PART_CTX_NATURE_FN) ctx = ctx_fn(block_);
    else if(nature == CMAP_PART_CTX_NATURE_PARAMS) ctx = ctx_params(block_);
    else if(nature == CMAP_PART_CTX_NATURE_PROC) ctx = ctx_proc(block_);
    else ctx = ctx_block(block_);
  }
  cmap_stack_ctx_push(&ctxs, ctx);

  upd(block_);

  nature = CMAP_PART_CTX_NATURE_BLOCK;
}

static char * pop()
{
  CMAP_PART_CTX ctx = cmap_stack_ctx_pop(&ctxs);
  if(!is_root(&ctx)) cur_ctx() -> block.next = NULL;

  free(ctx.block.prefix);
  cmap_part_keys_public.delete(&ctx.block.affecteds);

  cmap_part_kv_public.delete(&ctx.c.name2map);
  cmap_strings_public.delete(&ctx.c.params);

  cmap_strings_public.delete(&ctx.cmap.vars_loc);
  cmap_strings_public.delete(&ctx.cmap.vars_def);

  return ctx.block.instructions;
}

/*******************************************************************************
*******************************************************************************/

#define GET_BLOCK(what, type) \
static type * what(CMAP_PART_CTX * ctx) \
{ \
  if(ctx == NULL) ctx = cur_ctx(); \
  return &ctx -> block.what; \
}

#define GET_C(what, type) \
static type * what(CMAP_PART_CTX * ctx) \
{ \
  if(ctx == NULL) ctx = cur_ctx(); \
  return &ctx -> block.c -> c.what; \
}

#define GET_CMAP(what, type) \
static type * what(CMAP_PART_CTX * ctx) \
{ \
  if(ctx == NULL) ctx = cur_ctx(); \
  return &ctx -> block.c -> c.cmap -> cmap.what; \
}

#define TOGGLE_BLOCK(what) \
static void what() \
{ \
  cur_ctx() -> block.what = (1 == 1); \
} \
 \
static char is_##what() \
{ \
  if(cur_ctx() -> block.what) \
  { \
    cur_ctx() -> block.what = (1 == 0); \
    return (1 == 1); \
  } \
  return (1 == 0); \
}

#define ONLY_ONE_IS_C(what) \
static char is_##what() \
{ \
  if(!cur_ctx() -> block.c -> c.what) \
  { \
    cur_ctx() -> block.c -> c.what = (1 == 1); \
    return (1 == 0); \
  } \
  return (1 == 1); \
}

#define ONLY_ONE_SET_C(what) \
static void what() \
{ \
  cur_ctx() -> block.c -> c.what = (1 == 1); \
} \
 \
static char is_##what() \
{ \
  return cur_ctx() -> block.c -> c.what; \
}

/*******************************************************************************
*******************************************************************************/

GET_BLOCK(instructions, char *)

static const char * prefix()
{
  return cur_ctx() -> block.prefix;
}

TOGGLE_BLOCK(else_)

GET_BLOCK(fn_arg_names, CMAP_STRINGS *)

GET_BLOCK(affecteds, CMAP_PART_KEYS *)

/*******************************************************************************
*******************************************************************************/

ONLY_ONE_IS_C(definitions)
ONLY_ONE_IS_C(global_env)

ONLY_ONE_SET_C(return_)
ONLY_ONE_SET_C(return_fn)

GET_C(name2map, CMAP_PART_KV *)

GET_C(params, CMAP_STRINGS *)

/*******************************************************************************
*******************************************************************************/

GET_CMAP(vars_loc, CMAP_STRINGS *)
GET_CMAP(vars_def, CMAP_STRINGS *)

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX * c()
{
  return cur_ctx() -> block.c;
}

static CMAP_PART_CTX * cmap_prev(CMAP_PART_CTX * ctx)
{
  if(ctx == NULL) ctx = cur_ctx();
  return ctx -> block.c -> c.cmap -> cmap.prev;
}

static CMAP_PART_CTX * c_prev(CMAP_PART_CTX * ctx)
{
  if(ctx == NULL) ctx = cur_ctx();
  return ctx -> block.c -> c.prev;
}

static CMAP_PART_CTX * block_next(CMAP_PART_CTX * ctx)
{
  if(ctx == NULL) ctx = cur_ctx();
  return ctx -> block.next;
}

static CMAP_PART_CTX * last_block(CMAP_PART_CTX * ctx)
{
  while(block_next(ctx) != NULL) ctx = block_next(ctx);
  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static void restore(CMAP_PART_CTX * ctx)
{
  ctxs = cmap_stack_ctx_stack(ctx);
}

static CMAP_PART_CTX * bup(CMAP_PART_CTX * ctx)
{
  CMAP_PART_CTX * ret = cur_ctx();
  restore(ctx);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

#define NATURE_SET(NAME, name) nature_##name,

const CMAP_PART_CTX_PUBLIC cmap_part_ctx_public =
{
  CMAP_PART_CTX_NATURE_LOOP(NATURE_SET)
  is_params,
  push, pop,
  instructions, prefix, else_, is_else_, fn_arg_names, affecteds,
  is_definitions, is_global_env, return_, is_return_,
  return_fn, is_return_fn, name2map, params,
  vars_loc, vars_def,
  c, cmap_prev, c_prev, block_next, last_block,
  restore, bup
};
