
#include "cmap-fn.h"

#include <stdlib.h>
#include <string.h>
#include "cmap.h"
#include "cmap-mem.h"
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
  INTERNAL * internal = this -> internal;
  if(internal -> definitions == NULL)
  {
    internal -> definitions = cmap_map_public.create_root(proc_ctx);
    CMAP_INC_REFS(internal -> definitions);
  }
  return internal -> definitions;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * process(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_PROC_CTX * new_proc_ctx = CMAP_CALL(proc_ctx, create);

  INTERNAL * internal = this -> internal;

  CMAP_MAP * definitions = CMAP_CALL(new_proc_ctx, local_definitions);

  cmap_util_copy(definitions, internal -> definitions);

  return CMAP_CALL_ARGS(new_proc_ctx, delete,
    CMAP_CALL_ARGS(this, do_process, new_proc_ctx, map, args));
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * do_process(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  INTERNAL * internal = this -> internal;
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

static void delete(CMAP_LIFECYCLE * this)
{
  INTERNAL * internal = ((CMAP_FN *)this) -> internal;
  if(internal -> definitions != NULL) CMAP_DEC_REFS(internal -> definitions);
  CMAP_MEM_INSTANCE_FREE(internal);

  cmap_map_public.delete(this);
}

static CMAP_FN * init(CMAP_FN * this, CMAP_INITARGS * initargs,
  CMAP_FN_TPL process_)
{
  cmap_map_public.init((CMAP_MAP *)this, initargs);

  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  lc -> delete = delete;
  lc -> nested = nested;

  CMAP_MEM_INSTANCE_ALLOC_PTR(internal, INTERNAL);
  internal -> process = process_;
  internal -> definitions = NULL;

  this -> internal = internal;
  this -> require_definitions = require_definitions;
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

  CMAP_MEM_INSTANCE_ALLOC_PTR(this, CMAP_FN);
  return init(this, &initargs, process);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_FN_PUBLIC cmap_fn_public =
{
  create, init, delete,
  nested,
  require_definitions,
  process, do_process,
  new
};
