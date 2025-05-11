
#include "cmap-part.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-string.h"
#include "cmap-option.h"
#include "cmap-part-ctx.h"
#include "cmap-stack-define.h"
#include "cmap-part-dirty.h"
#include "cmap-part-kv.h"
#include "cmap-part-params.h"

/*******************************************************************************
*******************************************************************************/

CMAP_STACK_DEF(CTX, ctx, CMAP_PART_CTX)

/*******************************************************************************
*******************************************************************************/

#define PART_VAR(name) static char * name = NULL;

CMAP_PART_LOOP(PART_VAR)

static CMAP_STACK_CTX * ctxs = NULL;

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

static void push_instructions()
{
  CMAP_PART_CTX * ctx = NULL;
  if(ctxs != NULL) ctx = &ctxs -> v;

  cmap_stack_ctx_push(&ctxs, cmap_part_ctx_public.create(ctx));
  cmap_part_ctx_public.upd(&ctxs -> v);
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
  CMAP_PART_CTX_C * ctx = ctxs -> v.block.c;
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
  CMAP_PART_CTX_C * ctx = ctxs -> v.block.c;
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
  CMAP_PART_KV * name2map = data;
  cmap_part_kv_public.dirty(name2map, string);
}

static void mng_dirties(CMAP_PART_CTX_BLOCK * block)
{
  if((ctxs != NULL) && (block -> nature == CMAP_PART_CTX_NATURE_BLOCK))
  {
    CMAP_PART_KV * name2map = ctxs -> v.block.c -> name2map;
    cmap_strings_public.apply(block -> dirties, mng_dirties_apply, name2map);
  }
}

/*******************************************************************************
*******************************************************************************/

static char * pop_instructions()
{
  CMAP_PART_CTX ctx = cmap_stack_ctx_pop(&ctxs);

  mng_dirties(&ctx.block);

  cmap_part_ctx_public.delete(&ctx);

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

static void var(const char * name, const char * map)
{
  cmap_part_var_public.put(name, map, &ctxs -> v);
}

static void var_loc(const char * name, const char * map)
{
  cmap_part_var_public.put_loc(name, map, &ctxs -> v);
}

static char var_no_loc(const char * name, const char * map)
{
  return cmap_part_var_public.put_no_loc(name, map, &ctxs -> v);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_VAR_RET get_map(const char * name)
{
  return cmap_part_var_public.get(name, &ctxs -> v);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_STRINGS * get_vars_def()
{
  return cmap_part_var_public.defs(&ctxs -> v);
}

static CMAP_STRINGS * get_params()
{
  return cmap_part_params_public.this(&ctxs -> v);
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
}

/*******************************************************************************
*******************************************************************************/

#define PART_SET(name) name##_,

const CMAP_PART_PUBLIC cmap_part_public =
{
  clean,
  CMAP_PART_LOOP(PART_SET)
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
  var, var_loc, var_no_loc, get_map, get_vars_def, get_params,
  fn_arg_name, get_fn_arg_names, delete_fn_arg_names
};
