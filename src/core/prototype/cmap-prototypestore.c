
#include "cmap-prototypestore.h"

#include "cmap-kernel.h"
#include "cmap-mem.h"
#include "cmap-aisle.h"
#include "cmap-prototype-map.h"
#include "cmap-prototype-list.h"
#include "cmap-prototype-fn.h"
#include "cmap-prototype-string.h"
#include "cmap-prototype-int.h"
#include "cmap-prototype-double.h"
#include "cmap-prototype-ptr.h"

/*******************************************************************************
*******************************************************************************/

#define STRUCT_INTERNAL(type) CMAP_MAP * type##_;

typedef struct
{
  CMAP_PROTOTYPESTORE_LOOP(STRUCT_INTERNAL)
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

#define IMPL(type) \
static CMAP_MAP * require_##type(CMAP_PROTOTYPESTORE * this, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  INTERNAL * internal = (INTERNAL *)this -> internal; \
  if(internal -> type##_ == NULL) \
    cmap_prototype_##type##_public.require(&internal -> type##_, proc_ctx); \
  return internal -> type##_; \
} \
 \
static CMAP_MAP * type##_(CMAP_PROTOTYPESTORE * this, CMAP_PROC_CTX * proc_ctx) \
{ \
  INTERNAL * internal = (INTERNAL *)this -> internal; \
  if(internal -> type##_ == NULL) \
  { \
    require_##type(this, proc_ctx); \
    cmap_prototype_##type##_public.init(internal -> type##_, proc_ctx); \
  } \
  return internal -> type##_; \
}

CMAP_PROTOTYPESTORE_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

static CMAP_LIFECYCLE * delete(CMAP_LIFECYCLE * lc)
{
  CMAP_KERNEL_FREE(((CMAP_PROTOTYPESTORE *)lc) -> internal);
  return cmap_lifecycle_public.delete(lc);
}

#define INIT_INTERNAL(type) internal -> type##_ = NULL;

#define INIT_PS(type) \
  ps -> require_##type = require_##type; \
  ps -> type##_ = type##_;

static CMAP_PROTOTYPESTORE * create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MEM * mem = cmap_kernel_public.mem();
  CMAP_MEM_ALLOC_PTR(ps, CMAP_PROTOTYPESTORE, mem);

  cmap_lifecycle_public.init(&ps -> super, proc_ctx, CMAP_AISLE_GLOBAL);
  ps -> super.delete = delete;

  CMAP_MEM_ALLOC_PTR(internal, INTERNAL, mem);
  CMAP_PROTOTYPESTORE_LOOP(INIT_INTERNAL)

  ps -> internal = internal;
  CMAP_PROTOTYPESTORE_LOOP(INIT_PS)

  return ps;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPESTORE_PUBLIC cmap_prototypestore_public =
{
  create
};
