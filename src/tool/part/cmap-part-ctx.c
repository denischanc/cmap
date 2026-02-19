
#include "cmap-part-ctx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmap-part.h"
#include "cmap-string.h"
#include "cmap-stack-define.h"

/*******************************************************************************
*******************************************************************************/

#define NATURE_VAL(NAME, name, val) static const char NATURE_##NAME = val;

CMAP_PART_CTX_NATURE_LOOP(NATURE_VAL)

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char return_fn;

  CMAP_STRINGS * vars_loc, * vars_def;
} CTX_CMAP;

typedef struct
{
  char definitions, global_env, return_, * variables;

  CMAP_PART_KV * name2map;

  CMAP_STRINGS * params;
} CTX_C;

typedef struct
{
  char * instructions, * prefix, cmp_params;

  CMAP_STRINGS * fn_arg_names, * affecteds;
} CTX_BLOCK;

struct CMAP_PART_CTX
{
  CTX_CMAP cmap;
  CTX_C c;
  CTX_BLOCK block;

  int uid, uid_next, features;

  CMAP_PART_CTX * prev, * next;
};

CMAP_STACK_DEF(CTX, ctx, CMAP_PART_CTX)

/*******************************************************************************
*******************************************************************************/

static char nature = 0;

static CMAP_STACK_CTX * ctxs = NULL;

static int uid_ctx = 1;

/*******************************************************************************
*******************************************************************************/

static inline CMAP_PART_CTX * cur_ctx()
{
  return cmap_stack_ctx_v(ctxs);
}

/*******************************************************************************
*******************************************************************************/

const char * cmap_part_ctx_uid()
{
  CMAP_PART_CTX * ctx = cur_ctx();

  static char buffer[22];
  snprintf(buffer, sizeof(buffer), "%d_%d", ctx -> uid, ctx -> uid_next++);
  return buffer;
}

/*******************************************************************************
*******************************************************************************/

#define NATURE_IMPL(NAME, name, val) \
void cmap_part_ctx_nature_##name() \
{ \
  nature = val; \
}

CMAP_PART_CTX_NATURE_LOOP(NATURE_IMPL)

/*******************************************************************************
*******************************************************************************/

#define FEATURE_IMPL(name, val) \
static inline void set_feature_##name(CMAP_PART_CTX * ctx) \
{ \
  ctx -> features |= val; \
} \
 \
char cmap_part_ctx_is_feature_##name(CMAP_PART_CTX * ctx) \
{ \
  return ((ctx -> features & val) == val); \
}

CMAP_PART_CTX_FEATURE_LOOP(FEATURE_IMPL)

/*******************************************************************************
*******************************************************************************/

static void ctx_common(CMAP_PART_CTX * ctx)
{
  ctx -> uid = uid_ctx++;
  ctx -> uid_next = 0;
  ctx -> features = 0;
  ctx -> prev = cur_ctx();
  ctx -> next = NULL;

  ctx -> block.instructions = strdup("");
  ctx -> block.cmp_params = (1 == 1);
  ctx -> block.fn_arg_names = NULL;
  ctx -> block.affecteds = NULL;

  ctx -> c.definitions = (1 == 0);
  ctx -> c.global_env = (1 == 0);
  ctx -> c.return_ = (1 == 0);
  ctx -> c.variables = strdup("");
  ctx -> c.name2map = NULL;
  ctx -> c.params = NULL;

  ctx -> cmap.return_fn = (1 == 0);
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
  cmap_string_append(&ctx -> block.prefix, SPACE);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX create_ctx_fn_c(char params)
{
  CMAP_PART_CTX ctx;

  ctx_common(&ctx);
  prefix_fn(&ctx);

  set_feature_ctx_fn_c(&ctx);
  set_feature_ctx_c(&ctx);
  if(params) set_feature_fwd_name(&ctx);

  return ctx;
}

static CMAP_PART_CTX create_ctx_cmap(char return_fn)
{
  CMAP_PART_CTX ctx = create_ctx_fn_c(1 == 0);

  ctx.cmap.return_fn = return_fn;

  set_feature_ctx_cmap(&ctx);

  return ctx;
}

static CMAP_PART_CTX create_ctx_c()
{
  CMAP_PART_CTX ctx;

  ctx_common(&ctx);
  prefix_block(&ctx);

  set_feature_ctx_c(&ctx);

  return ctx;
}

static CMAP_PART_CTX create_ctx_block()
{
  CMAP_PART_CTX ctx;

  ctx_common(&ctx);
  prefix_block(&ctx);

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX create_ctx_root()
{
  return create_ctx_cmap(1 == 0);
}

static CMAP_PART_CTX create_ctx_fn()
{
  return create_ctx_cmap(1 == 1);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX create_ctx_iter()
{
  return create_ctx_fn_c(1 == 0);
}

char cmap_part_ctx_is_cmp_params();

static CMAP_PART_CTX create_ctx_cmp()
{
  return create_ctx_fn_c(cmap_part_ctx_is_cmp_params());
}

static CMAP_PART_CTX create_ctx_loop()
{
  return create_ctx_c();
}

/*******************************************************************************
*******************************************************************************/

void cmap_part_ctx_push()
{
  CMAP_PART_CTX ctx;
  if(ctxs == NULL) ctx = create_ctx_root();
  else
  {
    if(nature == NATURE_FN) ctx = create_ctx_fn();
    else if(nature == NATURE_ITER) ctx = create_ctx_iter();
    else if(nature == NATURE_CMP) ctx = create_ctx_cmp();
    else if(nature == NATURE_BLOCK) ctx = create_ctx_block();
    else if(nature == NATURE_LOOP) ctx = create_ctx_loop();
  }
  cmap_stack_ctx_push(&ctxs, ctx);

  CMAP_PART_CTX * cur_ctx_ = cur_ctx();
  if(cur_ctx_ -> prev != NULL) cur_ctx_ -> prev -> next = cur_ctx_;

  nature = NATURE_BLOCK;
}

char * cmap_part_ctx_pop()
{
  CMAP_PART_CTX ctx = cmap_stack_ctx_pop(&ctxs);
  if(cur_ctx() != NULL) cur_ctx() -> next = NULL;

  free(ctx.block.prefix);
  cmap_strings_delete(&ctx.block.fn_arg_names);
  cmap_strings_delete(&ctx.block.affecteds);

  cmap_part_kv_delete(&ctx.c.name2map);
  cmap_strings_delete(&ctx.c.params);

  cmap_strings_delete(&ctx.cmap.vars_loc);
  cmap_strings_delete(&ctx.cmap.vars_def);

  char * variables = ctx.c.variables;
  if(variables[0] != 0) cmap_string_append(&variables, "\n");
  cmap_string_prepend(&ctx.block.instructions, variables);
  free(variables);
  return ctx.block.instructions;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX * c_from_ctx(CMAP_PART_CTX * ctx)
{
  if(ctx == NULL) ctx = cur_ctx();
  while(!cmap_part_ctx_is_feature_ctx_c(ctx)) ctx = ctx -> prev;
  return ctx;
}

CMAP_PART_CTX * cmap_part_ctx_c()
{
  return c_from_ctx(NULL);
}

static CMAP_PART_CTX * fn_c_from_ctx(CMAP_PART_CTX * ctx)
{
  if(ctx == NULL) ctx = cur_ctx();
  while(!cmap_part_ctx_is_feature_ctx_fn_c(ctx)) ctx = ctx -> prev;
  return ctx;
}

CMAP_PART_CTX * cmap_part_ctx_fn_c()
{
  return fn_c_from_ctx(NULL);
}

static CMAP_PART_CTX * cmap_from_ctx(CMAP_PART_CTX * ctx)
{
  if(ctx == NULL) ctx = cur_ctx();
  while(!cmap_part_ctx_is_feature_ctx_cmap(ctx)) ctx = ctx -> prev;
  return ctx;
}

CMAP_PART_CTX * cmap_part_ctx_cmap()
{
  return cmap_from_ctx(NULL);
}

CMAP_PART_CTX * cmap_part_ctx_cmap_prev(CMAP_PART_CTX * ctx)
{
  ctx = cmap_from_ctx(ctx) -> prev;
  return (ctx != NULL) ? cmap_from_ctx(ctx) : NULL;
}

CMAP_PART_CTX * cmap_part_ctx_c_prev(CMAP_PART_CTX * ctx)
{
  ctx = c_from_ctx(ctx);
  return cmap_part_ctx_is_feature_ctx_cmap(ctx) ?
    NULL : c_from_ctx(ctx -> prev);
}

CMAP_PART_CTX * cmap_part_ctx_block_next(CMAP_PART_CTX * ctx)
{
  if(ctx == NULL) return NULL;

  ctx = ctx -> next;
  if(ctx == NULL) return NULL;

  return cmap_part_ctx_is_feature_ctx_c(ctx) ? NULL : ctx;
}

CMAP_PART_CTX * cmap_part_ctx_last_block(CMAP_PART_CTX * ctx)
{
  while(cmap_part_ctx_block_next(ctx) != NULL)
    ctx = cmap_part_ctx_block_next(ctx);
  return ctx;
}

CMAP_PART_CTX * cmap_part_ctx_block_prev(CMAP_PART_CTX * ctx)
{
  if(ctx == NULL) ctx = cur_ctx();
  return cmap_part_ctx_is_feature_ctx_c(ctx) ? NULL : ctx -> prev;
}

/*******************************************************************************
*******************************************************************************/

#define GET_BLOCK(what, type) \
type * cmap_part_ctx_##what(CMAP_PART_CTX * ctx) \
{ \
  if(ctx == NULL) ctx = cur_ctx(); \
  return &ctx -> block.what; \
}

#define GET_BLOCK_CONST(what, type) \
type cmap_part_ctx_##what(CMAP_PART_CTX * ctx) \
{ \
  if(ctx == NULL) ctx = cur_ctx(); \
  return ctx -> block.what; \
}

#define GET_C(what, type) \
type * cmap_part_ctx_##what(CMAP_PART_CTX * ctx) \
{ \
  ctx = c_from_ctx(ctx); \
  return &ctx -> c.what; \
}

#define GET_FN_C(what, type) \
type * cmap_part_ctx_##what(CMAP_PART_CTX * ctx) \
{ \
  ctx = fn_c_from_ctx(ctx); \
  return &ctx -> c.what; \
}

#define GET_CMAP(what, type) \
type * cmap_part_ctx_##what(CMAP_PART_CTX * ctx) \
{ \
  ctx = cmap_from_ctx(ctx); \
  return &ctx -> cmap.what; \
}

#define GET_CMAP_CONST(what, type) \
type cmap_part_ctx_##what(CMAP_PART_CTX * ctx) \
{ \
  ctx = cmap_from_ctx(ctx); \
  return ctx -> cmap.what; \
}

#define SET_RST_IS_BLOCK(what) \
void cmap_part_ctx_set_##what() \
{ \
  cur_ctx() -> block.what = (1 == 1); \
} \
 \
void cmap_part_ctx_rst_##what() \
{ \
  cur_ctx() -> block.what = (1 == 0); \
} \
 \
char cmap_part_ctx_is_##what() \
{ \
  return cur_ctx() -> block.what; \
}

#define ONLY_ONE_IS_FN_C(what) \
char cmap_part_ctx_is_##what##_n_set() \
{ \
  CMAP_PART_CTX * ctx = cmap_part_ctx_fn_c(); \
  if(!ctx -> c.what) \
  { \
    ctx -> c.what = (1 == 1); \
    return (1 == 0); \
  } \
  return (1 == 1); \
}

#define ONLY_ONE_SET_FN_C(what, name) \
void cmap_part_ctx_set_##name() \
{ \
  cmap_part_ctx_fn_c() -> c.what = (1 == 1); \
} \
 \
char cmap_part_ctx_is_##name() \
{ \
  return cmap_part_ctx_fn_c() -> c.what; \
}

/*******************************************************************************
*******************************************************************************/

GET_BLOCK(instructions, char *)

GET_BLOCK_CONST(prefix, const char *)

SET_RST_IS_BLOCK(cmp_params)

GET_BLOCK(fn_arg_names, CMAP_STRINGS *)

GET_BLOCK(affecteds, CMAP_STRINGS *)

/*******************************************************************************
*******************************************************************************/

GET_C(variables, char *)

GET_C(name2map, CMAP_PART_KV *)

/*******************************************************************************
*******************************************************************************/

ONLY_ONE_IS_FN_C(definitions)
ONLY_ONE_IS_FN_C(global_env)

ONLY_ONE_SET_FN_C(return_, return)

GET_FN_C(params, CMAP_STRINGS *)

/*******************************************************************************
*******************************************************************************/

GET_CMAP_CONST(return_fn, char)

GET_CMAP(vars_loc, CMAP_STRINGS *)
GET_CMAP(vars_def, CMAP_STRINGS *)

/*******************************************************************************
*******************************************************************************/

CMAP_STRINGS * cmap_part_ctx_prev_block_fn_arg_names(CMAP_PART_CTX * ctx)
{
  ctx = cmap_from_ctx(ctx) -> prev;
  return (ctx != NULL) ? ctx -> block.fn_arg_names : NULL;
}

/*******************************************************************************
*******************************************************************************/

CMAP_PART_CTX * cmap_part_ctx_split(CMAP_PART_CTX * ctx)
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

void cmap_part_ctx_join(CMAP_PART_CTX * ctx)
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

/*******************************************************************************
*******************************************************************************/

CMAP_PART_CTX * cmap_part_ctx_bup()
{
  CMAP_PART_CTX * ret = cur_ctx();
  ctxs = NULL;
  return ret;
}

void cmap_part_ctx_restore(CMAP_PART_CTX * ctx)
{
  ctxs = cmap_stack_ctx_stack(ctx);
}

/*******************************************************************************
*******************************************************************************/

void cmap_part_ctx_clean()
{
  while(ctxs != NULL) free(cmap_part_ctx_pop());
}
