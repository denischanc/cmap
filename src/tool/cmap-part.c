
#include "cmap-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-kv.h"
#include "cmap-option.h"
#include "cmap-stack-define.h"
#include "cmap-kv.h"

/*******************************************************************************
*******************************************************************************/

typedef struct CTX_CMAP CTX_CMAP;
typedef struct CTX_C CTX_C;
typedef struct CTX_BLOCK CTX_BLOCK;

struct CTX_CMAP
{
  CMAP_STRINGS * vars_loc, * vars_def;

  CTX_CMAP * prev;
};

struct CTX_C
{
  char definitions, global_env, return_, return_fn;

  CMAP_KV * name2map;
  CMAP_STRINGS * params;

  CTX_CMAP * cmap;

  CTX_BLOCK * block;

  CTX_C * prev;
};

struct CTX_BLOCK
{
  char * instructions, * prefix, else_, nature;

  CMAP_STRINGS * fn_arg_names, * dirties;

  CTX_C * c;
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
  CMAP_PART_CTX_NATURE_ROOT = 2, CMAP_PART_CTX_NATURE_BLOCK = 3,
  CMAP_PART_CTX_NATURE_PARAMS = 4;

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

static char is_ctx_cmap(char nature)
{
  return ((nature == CMAP_PART_CTX_NATURE_ROOT) ||
    (nature == CMAP_PART_CTX_NATURE_FN));
}

/*******************************************************************************
*******************************************************************************/

static CTX ctx_common()
{
  CTX ctx;

  ctx.block.fn_arg_names = NULL;
  ctx.block.dirties = NULL;

  ctx.c.name2map = NULL;
  ctx.c.params = NULL;

  ctx.cmap.vars_loc = NULL;
  ctx.cmap.vars_def = NULL;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CTX ctx_new()
{
  CTX ctx = ctx_common();

  ctx.block.instructions = strdup("");
  ctx.block.prefix = strdup(SPACE);
  ctx.block.else_ = (1 == 0);

  ctx.c.definitions = (1 == 0);
  ctx.c.global_env = (1 == 0);
  ctx.c.return_ = (1 == 0);

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CTX ctx_root_fn(char return_fn, CTX_CMAP * cmap_prev)
{
  CTX ctx = ctx_new();

  ctx.c.return_fn = return_fn;
  ctx.c.prev = NULL;

  ctx.cmap.prev = cmap_prev;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static void ctx_fn_upd_vars_loc_apply(const char * string, void * data)
{
  cmap_strings_public.add((CMAP_STRINGS **)data, string);
}

static void ctx_fn_upd_vars_loc(CMAP_STRINGS ** vars_loc)
{
  cmap_strings_public.add(vars_loc, "this");
  cmap_strings_public.add(vars_loc, "args");

  cmap_strings_public.apply(ctxs -> v.block.fn_arg_names,
    ctx_fn_upd_vars_loc_apply, vars_loc);
}

/*******************************************************************************
*******************************************************************************/

static CTX ctx_root()
{
  CTX ctx = ctx_root_fn((1 == 0), NULL);
  ctx.block.nature = CMAP_PART_CTX_NATURE_ROOT;
  return ctx;
}

static CTX ctx_fn(CTX_C * c_cur)
{
  CTX ctx = ctx_root_fn((1 == 1), c_cur -> cmap);
  ctx.block.nature = CMAP_PART_CTX_NATURE_FN;
  ctx_fn_upd_vars_loc(&ctx.cmap.vars_loc);
  return ctx;
}

static CTX ctx_dft(CTX_C * c_cur)
{
  CTX ctx = ctx_new();

  ctx.block.nature = CMAP_PART_CTX_NATURE_DFT;

  ctx.c.return_fn = (1 == 0);
  ctx.c.cmap = c_cur -> cmap;
  ctx.c.prev = c_cur;

  return ctx;
}

static CTX ctx_params(CTX_C * c_cur)
{
  CTX ctx = ctx_dft(c_cur);
  ctx.block.nature = CMAP_PART_CTX_NATURE_PARAMS;
  return ctx;
}

static CTX ctx_block(CTX_C * c_cur)
{
  CTX ctx = ctx_common();

  ctx.block.nature = CMAP_PART_CTX_NATURE_BLOCK;

  ctx.block.instructions = strdup("");
  ctx.block.prefix = strdup(ctxs -> v.block.prefix);
  cmap_string_public.append(&ctx.block.prefix, SPACE);
  ctx.block.else_ = (1 == 0);
  ctx.block.c = c_cur;

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
    else if(ctx_nature == CMAP_PART_CTX_NATURE_PARAMS) return ctx_params(c_cur);
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
    ctx -> c.block = &ctx -> block;
    if(is_ctx_cmap(ctx_nature)) ctx -> c.cmap = &ctx -> cmap;

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
  char ** instructions = &ctxs -> v.block.c -> block -> instructions;

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

static void mng_dirties_apply(const char * string, void * data)
{
  CMAP_KV * name2map = (CMAP_KV *)data;
  cmap_kv_public.dirty(name2map, string);
}

static void mng_dirties(CTX_BLOCK * block)
{
  if((ctxs != NULL) && (block -> nature == CMAP_PART_CTX_NATURE_BLOCK))
  {
    CMAP_KV * name2map = ctxs -> v.block.c -> name2map;
    cmap_strings_public.apply(block -> dirties, mng_dirties_apply, name2map);
  }

  cmap_strings_public.delete(&block -> dirties);
}

/*******************************************************************************
*******************************************************************************/

static char * pop_instructions()
{
  CTX ctx = cmap_stack_ctx_pop(&ctxs);

  mng_dirties(&ctx.block);

  cmap_kv_public.delete(&ctx.c.name2map);
  cmap_strings_public.delete(&ctx.c.params);
  cmap_strings_public.delete(&ctx.cmap.vars_loc);
  cmap_strings_public.delete(&ctx.cmap.vars_def);

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

static char is_else_n_rst()
{
  char ret = ctxs -> v.block.else_;
  ctxs -> v.block.else_ = (1 == 0);
  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void dirty(CTX_BLOCK * block, const char * name)
{
  if(block -> nature == CMAP_PART_CTX_NATURE_BLOCK)
    cmap_strings_public.add(&block -> dirties, name);
}

/*******************************************************************************
*******************************************************************************/

static void name2map(const char * name, const char * map)
{
  CTX_BLOCK * block = &ctxs -> v.block;

  cmap_kv_public.put(&block -> c -> name2map, name, map);

  dirty(block, name);
}

static void name2map_upd_prev(const char * name, const char * map)
{
  name2map(name, map);

  CTX_C * c = ctxs -> v.block.c -> prev;
  while(c != NULL)
  {
    cmap_kv_public.delete_key(&c -> name2map, name);

    c = c -> prev;
  }
}

/*******************************************************************************
*******************************************************************************/

static char is_var_loc(CTX_CMAP * cmap, const char * name)
{
  return cmap_strings_public.contains(cmap -> vars_loc, name);
}

static char is_var_def(CTX_CMAP * cmap, const char * name)
{
  return cmap_strings_public.contains(cmap -> vars_def, name);
}

static char is_var_loc_or_def(CTX_CMAP * cmap, const char * name)
{
  return (is_var_loc(cmap, name) || is_var_def(cmap, name));
}

/*******************************************************************************
*******************************************************************************/

static void var_loc(const char * name, const char * map)
{
  name2map_upd_prev(name, map);

  cmap_strings_public.add(&ctxs -> v.block.c -> cmap -> vars_loc, name);
}

/*******************************************************************************
*******************************************************************************/

static char is_var_loc_prev(const char * name)
{
  CTX_CMAP * cmap = ctxs -> v.block.c -> cmap -> prev;

  while(cmap != NULL)
  {
    if(is_var_loc(cmap, name)) return (1 == 1);

    cmap = cmap -> prev;
  }

  return (1 == 0);
}

static char var(const char * name, const char * map)
{
  name2map_upd_prev(name, map);

  CTX_CMAP * cmap = ctxs -> v.block.c -> cmap;
  if(is_var_loc_or_def(cmap, name)) return (1 == 1);

  if(is_var_loc_prev(name))
  {
    cmap_strings_public.add(&cmap -> vars_loc, name);
    return (1 == 1);
  }

  return (1 == 0);
}

/*******************************************************************************
*******************************************************************************/

static char maybe_var_def(CTX_CMAP * cmap, const char * name)
{
  if(is_var_loc_or_def(cmap, name)) return (1 == 1);

  CTX_CMAP * cmap_prev = cmap -> prev;
  if(cmap_prev == NULL) return (1 == 0);

  if(!maybe_var_def(cmap_prev, name)) return (1 == 0);

  cmap_strings_public.add(&cmap -> vars_def, name);
  return (1 == 1);
}

static void get_map_params(CMAP_PART_MAP_RET * ret, CTX_C * c,
  const char * name)
{
  const char * map = cmap_kv_public.get(c -> prev -> name2map, name);
  if(map == NULL) return;

  name2map(name, map);
  cmap_strings_public.add(&c -> params, name);

  ret -> map = map;
  ret -> dirty = (1 == 0);
}

static CMAP_PART_MAP_RET get_map(const char * name)
{
  CMAP_PART_MAP_RET ret;

  CTX_BLOCK * block = &ctxs -> v.block;
  CTX_C * c = block -> c;
  CMAP_KV * name2map = c -> name2map;
  CTX_CMAP * cmap = c -> cmap;

  ret.is_def = is_var_loc(cmap, name) ? (1 == 1) : maybe_var_def(cmap, name);

  ret.map = cmap_kv_public.get(name2map, name);
  if(ret.map != NULL)
  {
    ret.dirty = cmap_kv_public.is_dirty_n_rst(name2map, name);
    if(ret.dirty) dirty(block, name);
  }
  else if(block -> nature == CMAP_PART_CTX_NATURE_PARAMS)
    get_map_params(&ret, c, name);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * get_vars_def()
{
  return ctxs -> v.block.c -> cmap -> vars_def;
}

static CMAP_STRINGS * get_params()
{
  return ctxs -> v.block.c -> params;
}

/*******************************************************************************
*******************************************************************************/

static void fn_arg_name(char * name)
{
  cmap_strings_public.add(&ctxs -> v.block.fn_arg_names, name);
}

static CMAP_STRINGS * get_fn_arg_names()
{
  return ctxs -> v.block.fn_arg_names;
}

static void delete_fn_arg_names()
{
  cmap_strings_public.delete(&ctxs -> v.block.fn_arg_names);
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
  pop_instructions,
  pop_instructions_to_part,
  return_, is_return,
  return_fn, is_return_fn,
  add_include,
  set_else, is_else_n_rst,
  name2map, var_loc, var, get_map, get_vars_def, get_params,
  fn_arg_name, get_fn_arg_names, delete_fn_arg_names
};
