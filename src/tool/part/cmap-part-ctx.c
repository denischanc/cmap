
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
  macro(params, (1 << 2))

#define NATURE_VAL(NAME, name, val) static const char NATURE_##NAME = val;

CMAP_PART_CTX_NATURE_LOOP(NATURE_VAL)

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_STRINGS * vars_loc, * vars_def;
} CTX_CMAP;

typedef struct
{
  char definitions, global_env, return_, return_fn, * variables;

  CMAP_PART_KV * name2map;
  CMAP_STRINGS * params;
} CTX_C;

typedef struct
{
  char * instructions, * prefix, else_;

  CMAP_STRINGS * fn_arg_names, * affecteds;
} CTX_BLOCK;

struct CMAP_PART_CTX
{
  CTX_CMAP cmap;
  CTX_C c;
  CTX_BLOCK block;

  int features;

  CMAP_PART_CTX * prev, * next;
};

CMAP_STACK_DEF(CTX, ctx, CMAP_PART_CTX)

/*******************************************************************************
*******************************************************************************/

static char nature = 0;

static char cmp_params_ = (1 == 1);

static CMAP_STACK_CTX * ctxs = NULL;

/*******************************************************************************
*******************************************************************************/

static inline CMAP_PART_CTX * cur_ctx()
{
  return cmap_stack_ctx_v(ctxs);
}

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
  ctx -> prev = cur_ctx();
  ctx -> next = NULL;

  ctx -> block.instructions = strdup("");
  ctx -> block.else_ = (1 == 0);
  ctx -> block.fn_arg_names = NULL;
  ctx -> block.affecteds = NULL;

  ctx -> c.definitions = (1 == 0);
  ctx -> c.global_env = (1 == 0);
  ctx -> c.return_ = (1 == 0);
  ctx -> c.return_fn = (1 == 0);
  ctx -> c.variables = strdup("");
  ctx -> c.name2map = NULL;
  ctx -> c.params = NULL;

  ctx -> cmap.vars_loc = NULL;
  ctx -> cmap.vars_def = NULL;
}

static void prefix_fn(CMAP_PART_CTX * ctx)
{
  ctx -> block.prefix = strdup(SPACE);
}

static void prefix_block(CMAP_PART_CTX * ctx)
{
  ctx -> block.prefix = strdup(cur_ctx() -> block.prefix);
  cmap_string_public.append(&ctx -> block.prefix, SPACE);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX create_ctx_c()
{
  CMAP_PART_CTX ctx;

  ctx_common(&ctx);
  prefix_fn(&ctx);

  set_feature_ctx_c(&ctx);

  return ctx;
}

static CMAP_PART_CTX create_ctx_cmap()
{
  CMAP_PART_CTX ctx = create_ctx_c();
  set_feature_ctx_cmap(&ctx);
  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX create_ctx_root()
{
  return create_ctx_cmap();
}

static CMAP_PART_CTX create_ctx_fn()
{
  CMAP_PART_CTX ctx = create_ctx_cmap();
  ctx.c.return_fn = (1 == 1);
  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX create_ctx_iter()
{
  return create_ctx_c();
}

static CMAP_PART_CTX create_ctx_cmp()
{
  CMAP_PART_CTX ctx = create_ctx_c();
  if(cmp_params_) set_feature_params(&ctx);
  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX create_ctx_block()
{
  CMAP_PART_CTX ctx;

  ctx_common(&ctx);
  prefix_block(&ctx);

  cmap_strings_public.add_all(
    &ctx.block.affecteds, cur_ctx() -> block.affecteds);

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static void push()
{
  CMAP_PART_CTX ctx;
  if(ctxs == NULL) ctx = create_ctx_root();
  else
  {
    if(nature == NATURE_FN) ctx = create_ctx_fn();
    else if(nature == NATURE_ITER) ctx = create_ctx_iter();
    else if(nature == NATURE_CMP) ctx = create_ctx_cmp();
    else if(nature == NATURE_BLOCK) ctx = create_ctx_block();
  }
  cmap_stack_ctx_push(&ctxs, ctx);

  CMAP_PART_CTX * cur_ctx = cur_ctx();
  if(cur_ctx -> prev != NULL) cur_ctx -> prev -> next = cur_ctx;

  nature = NATURE_BLOCK;
}

static char * pop()
{
  CMAP_PART_CTX ctx = cmap_stack_ctx_pop(&ctxs);
  if(cur_ctx() != NULL) cur_ctx() -> next = NULL;

  free(ctx.block.prefix);
  cmap_strings_public.delete(&ctx.block.affecteds);

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

static CMAP_PART_CTX * c_from_ctx(CMAP_PART_CTX * ctx)
{
  if(ctx == NULL) ctx = cur_ctx();
  while(!is_feature_ctx_c(ctx)) ctx = ctx -> prev;
  return ctx;
}

static CMAP_PART_CTX * c()
{
  return c_from_ctx(NULL);
}

static CMAP_PART_CTX * cmap_from_ctx(CMAP_PART_CTX * ctx)
{
  if(ctx == NULL) ctx = cur_ctx();
  while(!is_feature_ctx_cmap(ctx)) ctx = ctx -> prev;
  return ctx;
}

static CMAP_PART_CTX * cmap()
{
  return cmap_from_ctx(NULL);
}

static CMAP_PART_CTX * cmap_prev(CMAP_PART_CTX * ctx)
{
  ctx = cmap_from_ctx(ctx) -> prev;
  return (ctx != NULL) ? cmap_from_ctx(ctx) : NULL;
}

static CMAP_PART_CTX * c_prev(CMAP_PART_CTX * ctx)
{
  ctx = c_from_ctx(ctx);
  return is_feature_ctx_cmap(ctx) ? NULL : c_from_ctx(ctx -> prev);
}

static CMAP_PART_CTX * block_next(CMAP_PART_CTX * ctx)
{
  if(ctx == NULL) return NULL;

  ctx = ctx -> next;
  if(ctx == NULL) return NULL;

  return is_feature_ctx_c(ctx) ? NULL : ctx;
}

static CMAP_PART_CTX * last_block(CMAP_PART_CTX * ctx)
{
  while(block_next(ctx) != NULL) ctx = block_next(ctx);
  return ctx;
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
  ctx = c_from_ctx(ctx); \
  return &ctx -> c.what; \
}

#define GET_C_CONST(what, type) \
static type what(CMAP_PART_CTX * ctx) \
{ \
  ctx = c_from_ctx(ctx); \
  return ctx -> c.what; \
}

#define GET_CMAP(what, type) \
static type * what(CMAP_PART_CTX * ctx) \
{ \
  ctx = cmap_from_ctx(ctx); \
  return &ctx -> cmap.what; \
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
  CMAP_PART_CTX * ctx = c(); \
  if(!ctx -> c.what) \
  { \
    ctx -> c.what = (1 == 1); \
    return (1 == 0); \
  } \
  return (1 == 1); \
}

#define ONLY_ONE_SET_C(what) \
static void what() \
{ \
  c() -> c.what = (1 == 1); \
} \
 \
static char is_##what() \
{ \
  return c() -> c.what; \
}

/*******************************************************************************
*******************************************************************************/

GET_BLOCK(instructions, char *)

GET_BLOCK_CONST(prefix, const char *)

TOGGLE_BLOCK(else_)

GET_BLOCK(fn_arg_names, CMAP_STRINGS *)

GET_BLOCK(affecteds, CMAP_STRINGS *)

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

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * prev_block_fn_arg_names(CMAP_PART_CTX * ctx)
{
  ctx = cmap_from_ctx(ctx) -> prev;
  return (ctx != NULL) ? ctx -> block.fn_arg_names : NULL;
}

/*******************************************************************************
*******************************************************************************/

static void restore(CMAP_PART_CTX * ctx)
{
  if(ctx != cur_ctx())
  {
    CMAP_PART_CTX * prev_ctx = ctx;
    while(prev_ctx -> prev != NULL) prev_ctx = prev_ctx -> prev;
    prev_ctx -> prev = cur_ctx();
    cur_ctx() -> next = prev_ctx;

    ctxs = cmap_stack_ctx_stack(ctx);
  }
}

static CMAP_PART_CTX * bup(CMAP_PART_CTX * ctx)
{
  CMAP_PART_CTX * ret = cur_ctx();

  if(ctx != ret)
  {
    ctx -> next -> prev = NULL;
    ctx -> next = NULL;
    ctxs = cmap_stack_ctx_stack(ctx);
  }

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
  c, cmap, cmap_prev, c_prev, block_next, last_block,
  instructions, prefix, else_, is_else_, fn_arg_names, affecteds,
  is_definitions, is_global_env, return_, is_return_, return_fn, variables,
  name2map, params,
  vars_loc, vars_def,
  prev_block_fn_arg_names,
  restore, bup
};
