
#include "cmap-part-ctx.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-stack-define.h"

/*******************************************************************************
*******************************************************************************/

#define FEATURE_LOOP(macro) \
  macro(ctx_cmap, (1 << 0)) \
  macro(ctx_c, (1 << 1)) \
  macro(fn_c, (1 << 2)) \
  macro(params, (1 << 3))

#define NATURE_VAL(NAME, name, val) static const char NATURE_##NAME = val;

CMAP_PART_CTX_NATURE_LOOP(NATURE_VAL)

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_STRINGS * vars_loc, * vars_def, * block_fn_arg_names;

  CMAP_PART_CTX * prev;
} CTX_CMAP;

typedef struct
{
  char definitions, global_env, return_, return_fn, * variables;

  CMAP_PART_KV * name2map;
  CMAP_STRINGS * params;

  CMAP_PART_CTX * cmap, * prev;
} CTX_C;

typedef struct
{
  char * instructions, * prefix, else_;

  CMAP_STRINGS * fn_arg_names;

  CMAP_PART_KEYS * affecteds;

  CMAP_PART_CTX * c, * next;
} CTX_BLOCK;

struct CMAP_PART_CTX
{
  CTX_CMAP cmap;
  CTX_C c;
  CTX_BLOCK block;

  int features;
};

CMAP_STACK_DEF(CTX, ctx, CMAP_PART_CTX)

/*******************************************************************************
*******************************************************************************/

static char nature = 0;

static char cmp_params_ = (1 == 1);

static CMAP_STACK_CTX * ctxs = NULL;

/*******************************************************************************
*******************************************************************************/

#define NATURE_IMPL(NAME, name, val) \
static void nature_##name() \
{ \
  nature = val; \
}

CMAP_PART_CTX_NATURE_LOOP(NATURE_IMPL)

/*******************************************************************************
*******************************************************************************/

static void cmp_params()
{
  cmp_params_ = (1 == 1);
}

static void cmp_no_params()
{
  cmp_params_ = (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

#define FEATURE_IMPL(name, val) \
static inline void set_feature_##name(CMAP_PART_CTX * ctx) \
{ \
  ctx -> features |= val; \
} \
 \
static char is_feature_##name(CMAP_PART_CTX * ctx) \
{ \
  return ((ctx -> features & val) == val); \
}

FEATURE_LOOP(FEATURE_IMPL)

/*******************************************************************************
*******************************************************************************/

static void ctx_common(CMAP_PART_CTX * ctx)
{
  ctx -> features = 0;

  ctx -> block.instructions = strdup("");
  ctx -> block.else_ = (1 == 0);
  ctx -> block.fn_arg_names = NULL;
  ctx -> block.affecteds = NULL;
  ctx -> block.next = NULL;

  ctx -> c.definitions = (1 == 0);
  ctx -> c.global_env = (1 == 0);
  ctx -> c.return_ = (1 == 0);
  ctx -> c.return_fn = (1 == 0);
  ctx -> c.variables = strdup("");
  ctx -> c.name2map = NULL;
  ctx -> c.params = NULL;

  ctx -> cmap.vars_loc = NULL;
  ctx -> cmap.vars_def = NULL;
  ctx -> cmap.block_fn_arg_names = NULL;
}

static void ctx_cmap(CMAP_PART_CTX * ctx, CTX_BLOCK * block_)
{
  set_feature_ctx_c(ctx);
  set_feature_ctx_cmap(ctx);

  ctx -> c.prev = NULL;

  ctx -> cmap.prev = (block_ == NULL) ? NULL : block_ -> c -> c.cmap;
}

static void ctx_c(CMAP_PART_CTX * ctx, CTX_BLOCK * block_)
{
  set_feature_ctx_c(ctx);

  CMAP_PART_CTX * c = block_ -> c;
  ctx -> c.cmap = c -> c.cmap;
  ctx -> c.prev = c;
}

static void ctx_fn_c(CMAP_PART_CTX * ctx)
{
  set_feature_fn_c(ctx);

  ctx -> block.prefix = strdup(SPACE);
}

static void ctx_block(CMAP_PART_CTX * ctx, CTX_BLOCK * block_)
{
  ctx -> block.prefix = strdup(block_ -> prefix);
  cmap_string_public.append(&ctx -> block.prefix, SPACE);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX create_ctx_cmap(CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx;

  ctx_common(&ctx);
  ctx_cmap(&ctx, block_);
  ctx_fn_c(&ctx);

  return ctx;
}

static CMAP_PART_CTX create_ctx_root()
{
  return create_ctx_cmap(NULL);
}

static CMAP_PART_CTX create_ctx_fn(CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx = create_ctx_cmap(block_);

  ctx.c.return_fn = (1 == 1);

  ctx.cmap.block_fn_arg_names = block_ -> fn_arg_names;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX create_ctx_c(CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx;

  ctx_common(&ctx);
  ctx_c(&ctx, block_);
  ctx_fn_c(&ctx);

  return ctx;
}

static CMAP_PART_CTX create_ctx_iter(CTX_BLOCK * block_)
{
  return create_ctx_c(block_);
}

static CMAP_PART_CTX create_ctx_cmp(CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx = create_ctx_c(block_);

  if(cmp_params_) set_feature_params(&ctx);

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX create_ctx_block(CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx;

  ctx_common(&ctx);
  ctx_block(&ctx, block_);

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
  if(is_feature_ctx_c(ctx))
  {
    ctx -> block.c = ctx;
    if(is_feature_ctx_cmap(ctx)) ctx -> c.cmap = ctx;
  }
  else block_ -> next = ctx;
}

static void push()
{
  CTX_BLOCK * block_ = NULL;

  CMAP_PART_CTX ctx;
  if(ctxs == NULL) ctx = create_ctx_root();
  else
  {
    block_ = &cur_ctx() -> block;

    if(nature == NATURE_FN) ctx = create_ctx_fn(block_);
    else if(nature == NATURE_ITER) ctx = create_ctx_iter(block_);
    else if(nature == NATURE_CMP) ctx = create_ctx_cmp(block_);
    else if(nature == NATURE_BLOCK) ctx = create_ctx_block(block_);
  }
  cmap_stack_ctx_push(&ctxs, ctx);

  upd(block_);

  nature = NATURE_BLOCK;
}

static char * pop()
{
  CMAP_PART_CTX ctx = cmap_stack_ctx_pop(&ctxs);
  if(ctxs != NULL) cur_ctx() -> block.next = NULL;

  free(ctx.block.prefix);
  cmap_part_keys_public.delete(&ctx.block.affecteds);

  cmap_part_kv_public.delete(&ctx.c.name2map);
  cmap_strings_public.delete(&ctx.c.params);

  cmap_strings_public.delete(&ctx.cmap.vars_loc);
  cmap_strings_public.delete(&ctx.cmap.vars_def);

  char * variables = ctx.c.variables;
  if(variables[0] != 0) cmap_string_public.append(&variables, "\n");
  cmap_string_public.prepend(&ctx.block.instructions, variables);
  free(variables);
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

#define GET_BLOCK_CONST(what, type) \
static type what(CMAP_PART_CTX * ctx) \
{ \
  if(ctx == NULL) ctx = cur_ctx(); \
  return ctx -> block.what; \
}

#define GET_C(what, type) \
static type * what(CMAP_PART_CTX * ctx) \
{ \
  if(ctx == NULL) ctx = cur_ctx(); \
  return &ctx -> block.c -> c.what; \
}

#define GET_C_CONST(what, type) \
static type what(CMAP_PART_CTX * ctx) \
{ \
  if(ctx == NULL) ctx = cur_ctx(); \
  return ctx -> block.c -> c.what; \
}

#define GET_CMAP(what, type) \
static type * what(CMAP_PART_CTX * ctx) \
{ \
  if(ctx == NULL) ctx = cur_ctx(); \
  return &ctx -> block.c -> c.cmap -> cmap.what; \
}

#define GET_CMAP_CONST(what, type) \
static type what(CMAP_PART_CTX * ctx) \
{ \
  if(ctx == NULL) ctx = cur_ctx(); \
  return ctx -> block.c -> c.cmap -> cmap.what; \
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

GET_BLOCK_CONST(prefix, const char *)

TOGGLE_BLOCK(else_)

GET_BLOCK(fn_arg_names, CMAP_STRINGS *)

GET_BLOCK(affecteds, CMAP_PART_KEYS *)

/*******************************************************************************
*******************************************************************************/

ONLY_ONE_IS_C(definitions)
ONLY_ONE_IS_C(global_env)

ONLY_ONE_SET_C(return_)
GET_C_CONST(return_fn, char)

GET_C(variables, char *)

GET_C(name2map, CMAP_PART_KV *)

GET_C(params, CMAP_STRINGS *)

/*******************************************************************************
*******************************************************************************/

GET_CMAP(vars_loc, CMAP_STRINGS *)
GET_CMAP(vars_def, CMAP_STRINGS *)

GET_CMAP_CONST(block_fn_arg_names, CMAP_STRINGS *)

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX * c()
{
  return cur_ctx() -> block.c;
}

static CMAP_PART_CTX * cmap()
{
  return c() -> c.cmap;
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

#define NATURE_SET(NAME, name, val) nature_##name,

const CMAP_PART_CTX_PUBLIC cmap_part_ctx_public =
{
  CMAP_PART_CTX_NATURE_LOOP(NATURE_SET)
  cmp_params, cmp_no_params,
  is_feature_params,
  push, pop,
  instructions, prefix, else_, is_else_, fn_arg_names, affecteds,
  is_definitions, is_global_env, return_, is_return_, return_fn, variables,
  name2map, params,
  vars_loc, vars_def, block_fn_arg_names,
  c, cmap, cmap_prev, c_prev, block_next, last_block,
  restore, bup
};
