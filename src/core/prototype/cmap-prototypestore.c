
#include "cmap-prototypestore.h"

#include "cmap-kernel.h"
#include "cmap-mem.h"
#include "cmap-prototype-map.h"
#include "cmap-prototype-list.h"
#include "cmap-prototype-fn.h"
#include "cmap-prototype-string.h"
#include "cmap-prototype-int.h"
#include "cmap-prototype-double.h"
#include "cmap-prototype-ptr.h"

/*******************************************************************************
*******************************************************************************/

#define STRUCT_INTERNAL(type) \
  CMAP_MAP * type##_; \
  char type##_ok;

typedef struct
{
  CMAP_PROTOTYPESTORE_LOOP(STRUCT_INTERNAL)
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static const char * nature(CMAP_LIFECYCLE * this)
{
  return "prototypestore";
}

/*******************************************************************************
*******************************************************************************/

#define NESTED_PUSH(type) \
  if(internal -> type##_ != NULL) \
    CMAP_CALL_ARGS(list, push, (CMAP_LIFECYCLE **)&internal -> type##_);

static void nested(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list)
{
  CMAP_PROTOTYPESTORE * this_ = (CMAP_PROTOTYPESTORE *)this;
  INTERNAL * internal = (INTERNAL *)(this_ + 1);
  CMAP_PROTOTYPESTORE_LOOP(NESTED_PUSH)

  cmap_lifecycle_public.nested(this, list);
}

/*******************************************************************************
*******************************************************************************/

#define IMPL(type) \
static CMAP_MAP * require_##type(CMAP_PROTOTYPESTORE * this, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  if(internal -> type##_ == NULL) \
  { \
    cmap_prototype_##type##_public.require(&internal -> type##_, proc_ctx); \
    CMAP_INC_REFS(internal -> type##_); \
  } \
  return internal -> type##_; \
} \
 \
static CMAP_MAP * type##_(CMAP_PROTOTYPESTORE * this, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  INTERNAL * internal = (INTERNAL *)(this + 1); \
  if((internal -> type##_ == NULL) || !internal -> type##_ok) \
  { \
    require_##type(this, proc_ctx); \
 \
    internal -> type##_ok = CMAP_T; \
    cmap_prototype_##type##_public.init(internal -> type##_, proc_ctx); \
  } \
  return internal -> type##_; \
}

CMAP_PROTOTYPESTORE_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

#define DEC_REFS(type) \
  if(internal -> type##_ != NULL) CMAP_DEC_REFS(internal -> type##_);

static void delete(CMAP_LIFECYCLE * lc)
{
  INTERNAL * internal = (INTERNAL *)(((CMAP_PROTOTYPESTORE *)lc) + 1);
  CMAP_PROTOTYPESTORE_LOOP(DEC_REFS)

  cmap_lifecycle_public.delete(lc);
}

#define INIT_INTERNAL(type) \
  internal -> type##_ = NULL; \
  internal -> type##_ok = CMAP_F;

#define INIT_THIS(type) \
  this -> require_##type = require_##type; \
  this -> type##_ = type##_;

static CMAP_PROTOTYPESTORE * create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MEM * mem = CMAP_KERNEL_MEM;
  CMAP_PROTOTYPESTORE * this = (CMAP_PROTOTYPESTORE *)mem -> alloc(
    sizeof(CMAP_PROTOTYPESTORE) + sizeof(INTERNAL));

  CMAP_INITARGS initargs;
  initargs.allocator = NULL;
  initargs.proc_ctx = proc_ctx;
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)this;
  cmap_lifecycle_public.init(lc, &initargs);
  lc -> delete = delete;
  lc -> nature = nature;
  lc -> nested = nested;

  INTERNAL * internal = (INTERNAL *)(this + 1);
  CMAP_PROTOTYPESTORE_LOOP(INIT_INTERNAL)

  CMAP_PROTOTYPESTORE_LOOP(INIT_THIS)

  return this;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPESTORE_PUBLIC cmap_prototypestore_public =
{
  create
};
