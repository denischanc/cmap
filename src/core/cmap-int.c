
#include "cmap-int.h"

#include "cmap-mem.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int64_t val;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

const char * CMAP_INT_NATURE = "int";

/*******************************************************************************
*******************************************************************************/

static int64_t get(CMAP_INT * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return internal -> val;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_INT * set(CMAP_INT * this, int64_t val)
{
  INTERNAL * internal = this -> internal;
  internal -> val = val;
  return this;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_LIFECYCLE * this)
{
  CMAP_MEM_VAR_FREE(((CMAP_INT *)this) -> internal);

  cmap_map_public.delete(this);
}

static CMAP_INT * init(CMAP_INT * this, CMAP_INITARGS * initargs, int64_t val)
{
  cmap_map_public.init((CMAP_MAP *)this, initargs);

  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  lc -> delete = delete;

  CMAP_MEM_VAR_ALLOC_PTR(internal, INTERNAL);
  internal -> val = val;

  this -> internal = internal;
  this -> get = get;
  this -> set = set;

  return this;
}

static CMAP_INT * create(int64_t val, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_INITARGS initargs;
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  initargs.nature = CMAP_INT_NATURE;
  initargs.prototype = CMAP_CALL_ARGS(ps, int_, proc_ctx);
  initargs.allocator = NULL;
  initargs.proc_ctx = proc_ctx;

  CMAP_MEM_VAR_ALLOC_PTR(this, CMAP_INT);
  return init(this, &initargs, val);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_INT_PUBLIC cmap_int_public =
{
  create, init, delete,
  get, set
};
