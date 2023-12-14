
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

static const char * nature(CMAP_MAP * this)
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

static void delete(CMAP_INT * int_)
{
  CMAP_KERNEL_FREE(int_ -> internal);

  cmap_map_public.delete(&int_ -> super);
}

static void delete_(CMAP_LIFECYCLE * int_)
{
  delete((CMAP_INT *)int_);
}

#define OP_STEP_INIT(name, op) int_ -> name = name;

static void init(CMAP_INT * int_, int64_t val)
{
  CMAP_MAP * super = &int_ -> super;
  super -> nature = nature;
  super -> super.delete = delete_;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> val = val;

  int_ -> internal = internal;
  int_ -> get = get;
  CMAP_INT_OP_LOOP(OP_STEP_INIT)
  CMAP_INT_STEP_LOOP(OP_STEP_INIT)
}

static CMAP_INT * create(int64_t val, CMAP_PROC_CTX * proc_ctx)
{
  CMAP_PROTOTYPESTORE * ps = CMAP_CALL(proc_ctx, prototypestore);
  CMAP_MAP * prototype_int = CMAP_CALL_ARGS(ps, int_, proc_ctx);
  CMAP_INT * int_ = CMAP_PROTOTYPE_NEW(prototype_int, CMAP_INT, proc_ctx);
  init(int_, val);
  return int_;
}

/*******************************************************************************
*******************************************************************************/

#define OP_STEP_SET(name, op) name,

const CMAP_INT_PUBLIC cmap_int_public =
{
  create,
  init,
  delete,
  get,
  CMAP_INT_OP_LOOP(OP_STEP_SET)
  CMAP_INT_STEP_LOOP(OP_STEP_SET)
};
