
#include "cmap-fn.h"

#include <stdlib.h>
#include <string.h>
#include "cmap.h"
#include "cmap-kernel.h"
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

  CMAP_SLIST_CHAR_PTR * arg_names;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_FN_NATURE = "fn";

const char * CMAP_PROTOTYPE_NAME = "prototype";

/*******************************************************************************
*******************************************************************************/

static void nested(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list)
{
  INTERNAL * internal = ((CMAP_FN *)this) -> internal;
  if(internal -> definitions != NULL)
    CMAP_CALL_ARGS(list, push, (CMAP_LIFECYCLE **)&internal -> definitions);

  cmap_map_public.nested(this, list);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * require_definitions(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> definitions == NULL)
  {
    internal -> definitions = cmap_map_public.create_root(proc_ctx);
    CMAP_INC_REFS(internal -> definitions);
  }
  return internal -> definitions;
}

/*******************************************************************************
*******************************************************************************/

static void add_arg_name(CMAP_FN * this, const char * arg_name)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> arg_names == NULL)
    internal -> arg_names = cmap_slist_char_ptr_public.create(1 << 4);
  CMAP_CALL_ARGS(internal -> arg_names, push,
    cmap_util_public.strdup(arg_name));
}

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int off;
  CMAP_LIST * args;
  CMAP_MAP * definitions;
} ARG_NAME_APPLY_DATA;

static void arg_name_apply(char ** arg_name, void * data)
{
  ARG_NAME_APPLY_DATA * data_ = data;
  CMAP_SET(data_ -> definitions, *arg_name,
    CMAP_LIST_GET(data_ -> args, data_ -> off++));
}

static CMAP_MAP * process(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_PROC_CTX * new_proc_ctx = CMAP_CALL(proc_ctx, new_level);

  INTERNAL * internal = (INTERNAL *)this -> internal;

  CMAP_MAP * definitions = CMAP_CALL(new_proc_ctx, local_definitions);

  cmap_util_public.copy(definitions, internal -> definitions);

  if(internal -> arg_names != NULL)
  {
    ARG_NAME_APPLY_DATA data = { 0, args, definitions };
    CMAP_APPLY(internal -> arg_names, arg_name_apply, &data);
  }

  return CMAP_CALL_ARGS(new_proc_ctx, delete,
    CMAP_CALL_ARGS(this, do_process, new_proc_ctx, map, args));
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
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * map = NULL;

  CMAP_MAP * prototype = CMAP_GET(this, CMAP_PROTOTYPE_NAME);
  if(prototype != NULL) map = CMAP_PROTOTYPE_NEW(prototype, proc_ctx);
  else map = CMAP_MAP(proc_ctx);

  CMAP_FN_PROC(this, proc_ctx, map, args);

  return map;
}

/*******************************************************************************
*******************************************************************************/

static void delete_arg_name_apply(char ** arg_name, void * data)
{
  CMAP_KERNEL_FREE(*arg_name);
}

static void delete(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = (INTERNAL *)((CMAP_FN *)this) -> internal;
  if(internal -> definitions != NULL) CMAP_DEC_REFS(internal -> definitions);
  if(internal -> arg_names != NULL)
  {
    CMAP_CALL_ARGS(internal -> arg_names, apply, delete_arg_name_apply, NULL);
    CMAP_CALL(internal -> arg_names, delete);
  }
  CMAP_KERNEL_FREE(internal);

  cmap_map_public.delete(this);
}

static CMAP_FN * init(CMAP_FN * this, CMAP_INITARGS * initargs,
  CMAP_FN_TPL process_)
{
  cmap_map_public.init((CMAP_MAP *)this, initargs);

  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  lc -> delete = delete;
  lc -> nested = nested;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> process = process_;
  internal -> definitions = NULL;
  internal -> arg_names = NULL;

  this -> internal = internal;
  this -> require_definitions = require_definitions;
  this -> add_arg_name = add_arg_name;
  this -> process = process;
  this -> do_process = do_process;
  this -> new = new;

  return this;
}

static CMAP_FN * create(CMAP_FN_TPL process, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  initargs.nature = CMAP_FN_NATURE;
  initargs.prototype = CMAP_CALL_ARGS(ps, fn_, proc_ctx);
  initargs.allocator = NULL;
  initargs.proc_ctx = proc_ctx;

  CMAP_FN * this = (CMAP_FN *)CMAP_KERNEL_MEM -> alloc(sizeof(CMAP_FN));
  return init(this, &initargs, process);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_FN_PUBLIC cmap_fn_public =
{
  create, init, delete,
  nested,
  require_definitions,
  add_arg_name,
  process, do_process,
  new
};
