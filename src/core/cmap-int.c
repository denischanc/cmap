
#include "cmap-int.h"

#include "cmap-kernel.h"
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

#define OP_STEP_FN_NAME(name) op_step_##name

#define OP_IMPL(name, op) \
static CMAP_INT * OP_STEP_FN_NAME(name)(CMAP_INT * this, int64_t val) \
{ \
  INTERNAL * internal = (INTERNAL *)this -> internal; \
  internal -> val op val; \
  return this; \
}

CMAP_INT_OP_LOOP(OP_IMPL)

/*******************************************************************************
*******************************************************************************/

#define STEP_IMPL(name, op) \
static CMAP_INT * OP_STEP_FN_NAME(name)(CMAP_INT * this) \
{ \
  INTERNAL * internal = (INTERNAL *)this -> internal; \
  internal -> val op; \
  return this; \
}

CMAP_INT_STEP_LOOP(STEP_IMPL)

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_LIFECYCLE * this)
{
  CMAP_KERNEL_FREE(((CMAP_INT *)this) -> internal);

  cmap_map_public.delete(this);
}

#define OP_STEP_INIT(name, op) this -> name = OP_STEP_FN_NAME(name);

static CMAP_INT * init(CMAP_INT * this, CMAP_INITARGS * initargs, int64_t val)
{
  cmap_map_public.init((CMAP_MAP *)this, initargs);

  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  lc -> delete = delete;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> val = val;

  this -> internal = internal;
  this -> get = get;
  CMAP_INT_OP_LOOP(OP_STEP_INIT)
  CMAP_INT_STEP_LOOP(OP_STEP_INIT)

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

  CMAP_INT * this = (CMAP_INT *)CMAP_KERNEL_MEM -> alloc(sizeof(CMAP_INT));
  return init(this, &initargs, val);
}

/*******************************************************************************
*******************************************************************************/

#define OP_STEP_SET(name, op) OP_STEP_FN_NAME(name),

const CMAP_INT_PUBLIC cmap_int_public =
{
  create, init, delete,
  get,
  CMAP_INT_OP_LOOP(OP_STEP_SET)
  CMAP_INT_STEP_LOOP(OP_STEP_SET)
};
