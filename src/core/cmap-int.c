
#include "cmap-int.h"

#include "cmap-kernel.h"
#include "cmap-prototypestore.h"
#include "cmap-proc-ctx.h"
#include "cmap-log.h"

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

static const char * nature(CMAP_LIFECYCLE * this)
{
  return CMAP_INT_NATURE;
}

/*******************************************************************************
*******************************************************************************/

static int64_t get(CMAP_INT * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  return internal -> val;
}

/*******************************************************************************
*******************************************************************************/

#define OP_IMPL(name, op) \
static CMAP_INT * name(CMAP_INT * this, int64_t val) \
{ \
  INTERNAL * internal = (INTERNAL *)this -> internal; \
  internal -> val op val; \
  return this; \
}

CMAP_INT_OP_LOOP(OP_IMPL)

/*******************************************************************************
*******************************************************************************/

#define STEP_IMPL(name, op) \
static CMAP_INT * name(CMAP_INT * this) \
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
  cmap_log_public.debug("[%p][%s] deletion", this, CMAP_NATURE(this));

  CMAP_KERNEL_FREE(((CMAP_INT *)this) -> internal);

  cmap_map_public.delete(this);
}

#define OP_STEP_INIT(name, op) this -> name = name;

static void init(CMAP_INT * this, int64_t val)
{
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  lc -> delete = delete;
  lc -> nature = nature;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> val = val;

  this -> internal = internal;
  this -> get = get;
  CMAP_INT_OP_LOOP(OP_STEP_INIT)
  CMAP_INT_STEP_LOOP(OP_STEP_INIT)
}

static CMAP_INT * create(int64_t val, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  CMAP_MAP * prototype_int = CMAP_CALL_ARGS(ps, int_, proc_ctx);
  CMAP_INT * this = CMAP_PROTOTYPE_NEW(prototype_int, CMAP_INT, proc_ctx);
  init(this, val);
  return this;
}

/*******************************************************************************
*******************************************************************************/

#define OP_STEP_SET(name, op) name,

const CMAP_INT_PUBLIC cmap_int_public =
{
  create, init, delete,
  get,
  CMAP_INT_OP_LOOP(OP_STEP_SET)
  CMAP_INT_STEP_LOOP(OP_STEP_SET)
};
