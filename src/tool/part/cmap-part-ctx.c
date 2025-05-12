
#include "cmap-part-ctx.h"

#include <stdlib.h>
#include <string.h>
#include "cmap-part.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

const char CMAP_PART_CTX_NATURE_FN = 0, CMAP_PART_CTX_NATURE_ROOT = 1,
  CMAP_PART_CTX_NATURE_BLOCK = 2, CMAP_PART_CTX_NATURE_PARAMS = 3;

static char nature = CMAP_PART_CTX_NATURE_BLOCK;

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

static char is_cmap(char nature)
{
  return ((nature == CMAP_PART_CTX_NATURE_ROOT) ||
    (nature == CMAP_PART_CTX_NATURE_FN));
}

static char is_new(char nature)
{
  return (nature != CMAP_PART_CTX_NATURE_BLOCK);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX common()
{
  CMAP_PART_CTX ctx;

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

static CMAP_PART_CTX new()
{
  CMAP_PART_CTX ctx = common();

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

static CMAP_PART_CTX root_fn(char return_fn, CMAP_PART_CTX_CMAP * cmap_prev)
{
  CMAP_PART_CTX ctx = new();

  ctx.c.return_fn = return_fn;
  ctx.c.prev = NULL;

  ctx.cmap.prev = cmap_prev;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX root()
{
  CMAP_PART_CTX ctx = root_fn((1 == 0), NULL);
  ctx.block.nature = CMAP_PART_CTX_NATURE_ROOT;
  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX fn(CMAP_PART_CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx = root_fn((1 == 1), block_ -> c -> cmap);

  ctx.block.nature = CMAP_PART_CTX_NATURE_FN;

  CMAP_PART_KV ** name2map = &ctx.c.name2map;
  cmap_part_kv_public.put(name2map, NULL, "this", "this");
  cmap_part_kv_public.put(name2map, NULL, "args", "args");

  ctx.cmap.vars_loc = cmap_strings_public.clone(block_ -> fn_arg_names);

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX params(CMAP_PART_CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx = new();

  ctx.block.nature = CMAP_PART_CTX_NATURE_PARAMS;

  ctx.c.return_fn = (1 == 0);
  CMAP_PART_CTX_C * c_cur = block_ -> c;
  ctx.c.cmap = c_cur -> cmap;
  ctx.c.prev = c_cur;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX block(CMAP_PART_CTX_BLOCK * block_)
{
  CMAP_PART_CTX ctx = common();

  ctx.block.nature = CMAP_PART_CTX_NATURE_BLOCK;

  ctx.block.instructions = strdup("");
  ctx.block.prefix = strdup(block_ -> prefix);
  cmap_string_public.append(&ctx.block.prefix, SPACE);
  ctx.block.else_ = (1 == 0);
  ctx.block.c = block_ -> c;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_PART_CTX create(CMAP_PART_CTX * ctx)
{
  CMAP_PART_CTX ret;

  if(ctx == NULL) ret = root();
  else
  {
    CMAP_PART_CTX_BLOCK * block_ = &ctx -> block;
    if(nature == CMAP_PART_CTX_NATURE_FN) ret = fn(block_);
    else if(nature == CMAP_PART_CTX_NATURE_PARAMS) ret = params(block_);
    else ret = block(block_);
  }

  nature = CMAP_PART_CTX_NATURE_BLOCK;

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static void upd(CMAP_PART_CTX * ctx)
{
  char nature = ctx -> block.nature;
  if(is_new(nature))
  {
    ctx -> block.c = &ctx -> c;
    ctx -> c.block = &ctx -> block;
    if(is_cmap(nature)) ctx -> c.cmap = &ctx -> cmap;
  }
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_PART_CTX * ctx)
{
  free(ctx -> block.prefix);
  cmap_part_keys_public.delete(&ctx -> block.dirties);

  cmap_part_kv_public.delete(&ctx -> c.name2map);
  cmap_strings_public.delete(&ctx -> c.params);

  cmap_strings_public.delete(&ctx -> cmap.vars_loc);
  cmap_strings_public.delete(&ctx -> cmap.vars_def);
}

/*******************************************************************************
*******************************************************************************/

#define NATURE_SET(NAME, name) nature_##name,

const CMAP_PART_CTX_PUBLIC cmap_part_ctx_public =
{
  CMAP_PART_CTX_NATURE_LOOP(NATURE_SET)
  create, upd,
  delete
};
