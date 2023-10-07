
#include "cmap-fn.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-kernel.h"
#include "cmap-string.h"
#include "cmap-util.h"
#include "cmap-proc-ctx.h"
#include "cmap-prototypestore.h"
#include "cmap-list.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_FN_TPL process;

  CMAP_MAP * definitions;

  CMAP_LIST * arg_names;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_FN_NATURE = "fn";

const char * CMAP_PROTOTYPE_NAME = "prototype";

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_MAP * this)
{
  return CMAP_FN_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * require_definitions(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> definitions == NULL)
    internal -> definitions = cmap_map_public.create_root(proc_ctx, NULL);
  return internal -> definitions;
}

/*******************************************************************************
*******************************************************************************/

static void add_arg_name(CMAP_FN * this, const char * arg_name,
  CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> arg_names == NULL)
    internal -> arg_names = CMAP_LIST(0, proc_ctx, NULL);

  CMAP_LIST_PUSH(internal -> arg_names,
    CMAP_STRING(arg_name, 0, proc_ctx, NULL));
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * process(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_CALL(proc_ctx, push_local);

  INTERNAL * internal = (INTERNAL *)this -> internal;

  CMAP_MAP * definitions = CMAP_CALL(proc_ctx, local_definitions);
  CMAP_SET(definitions, "this", map);
  CMAP_SET(definitions, "args", args);

  CMAP_LIST * arg_names = internal -> arg_names;
  if(arg_names != NULL)
  {
    int arg_names_size = CMAP_CALL(arg_names, size);
    for(int i = 0; i < arg_names_size; i++)
    {
      CMAP_STRING * arg_name = (CMAP_STRING *)CMAP_LIST_GET(arg_names, i);
      CMAP_SET(definitions, CMAP_CALL(arg_name, val), CMAP_LIST_GET(args, i));
    }
  }

  cmap_util_public.copy(definitions, internal -> definitions);

  CMAP_MAP * ret = CMAP_CALL_ARGS(this, do_process, proc_ctx, map, args);

  CMAP_CALL(proc_ctx, pop_local);

  return ret;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * do_process(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return internal -> process(proc_ctx, map, args);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * new(CMAP_FN * this, CMAP_LIST * args,
  CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  CMAP_MAP * map = NULL;

  CMAP_MAP * prototype = CMAP_GET(this, CMAP_PROTOTYPE_NAME);
  if(prototype != NULL)
    map = CMAP_PROTOTYPE_NEW(prototype, CMAP_MAP, proc_ctx, aisle);
  else map = CMAP_MAP(proc_ctx, aisle);

  if(aisle != NULL)
  {
    CMAP_STRING * aisle_string = CMAP_STRING(aisle, 0, proc_ctx, aisle);
    CMAP_SET(map, "cmap-aisle", aisle_string);
  }

  CMAP_FN_PROC(this, proc_ctx, map, args);

  return map;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LIFECYCLE * delete(CMAP_FN * fn)
{
  INTERNAL * internal = (INTERNAL *)fn -> internal;

  if(internal -> definitions != NULL) CMAP_DELETE(internal -> definitions);
  if(internal -> arg_names != NULL)
    cmap_util_public.delete_list_n_vals(internal -> arg_names);
  CMAP_KERNEL_FREE(internal);

  return cmap_map_public.delete(&fn -> super);
}

static CMAP_LIFECYCLE * delete_(CMAP_LIFECYCLE * fn)
{
  return delete((CMAP_FN *)fn);
}

static void init(CMAP_FN * fn, CMAP_FN_TPL process_)
{
  CMAP_MAP * super = &fn -> super;
  super -> nature = nature;
  super -> super.delete = delete_;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> process = process_;
  internal -> definitions = NULL;
  internal -> arg_names = NULL;

  fn -> internal = internal;
  fn -> require_definitions = require_definitions;
  fn -> add_arg_name = add_arg_name;
  fn -> process = process;
  fn -> do_process = do_process;
  fn -> new = new;
}

static CMAP_FN * create(CMAP_FN_TPL process, CMAP_PROC_CTX * proc_ctx,
  const char * aisle)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  CMAP_MAP * prototype_fn = CMAP_CALL_ARGS(ps, fn_, proc_ctx);
  CMAP_FN * fn = CMAP_PROTOTYPE_NEW(prototype_fn, CMAP_FN, proc_ctx, aisle);
  init(fn, process);
  return fn;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_FN_PUBLIC cmap_fn_public =
{
  create,
  init,
  delete,
  add_arg_name,
  process,
  do_process,
  new
};
