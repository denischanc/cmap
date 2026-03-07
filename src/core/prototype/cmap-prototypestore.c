
#include "cmap-prototypestore.h"

#include <stddef.h>
#include "cmap-mem.h"
#include "cmap-prototype-map.h"
#include "cmap-prototype-list.h"
#include "cmap-prototype-fn.h"
#include "cmap-prototype-string.h"
#include "cmap-prototype-int.h"
#include "cmap-prototype-double.h"
#include "cmap-prototype-ptr.h"
#include "cmap-lifecycle.h"

/*******************************************************************************
*******************************************************************************/

#define STRUCT(type) \
  CMAP_MAP * type##_; \
  char type##_ok;

struct CMAP_PROTOTYPESTORE
{
  CMAP_PROTOTYPESTORE_LOOP(STRUCT)
};

/*******************************************************************************
*******************************************************************************/

#define IMPL(type) \
CMAP_MAP * cmap_prototypestore_require_##type(CMAP_PROTOTYPESTORE * ps, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  if(ps -> type##_ == NULL) \
  { \
    cmap_prototype_##type##_require(&ps -> type##_, proc_ctx); \
    CMAP_INC_REFS(ps -> type##_); \
  } \
  return ps -> type##_; \
} \
 \
CMAP_MAP * cmap_prototypestore_##type(CMAP_PROTOTYPESTORE * ps, \
  CMAP_PROC_CTX * proc_ctx) \
{ \
  if((ps -> type##_ == NULL) || !ps -> type##_ok) \
  { \
    cmap_prototypestore_require_##type(ps, proc_ctx); \
 \
    ps -> type##_ok = CMAP_T; \
    cmap_prototype_##type##_init(ps -> type##_, proc_ctx); \
  } \
  return ps -> type##_; \
}

CMAP_PROTOTYPESTORE_LOOP(IMPL)

/*******************************************************************************
*******************************************************************************/

#define DEC_REFS(type) \
  if(ps -> type##_ != NULL) CMAP_DEC_REFS(ps -> type##_);

void cmap_prototypestore_delete(CMAP_PROTOTYPESTORE * ps)
{
  CMAP_PROTOTYPESTORE_LOOP(DEC_REFS)

  cmap_mem_free(ps);
}

#define INIT(type) \
  ps -> type##_ = NULL; \
  ps -> type##_ok = CMAP_F;

CMAP_PROTOTYPESTORE * cmap_prototypestore_create(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MEM_ALLOC_PTR(ps, CMAP_PROTOTYPESTORE);
  CMAP_PROTOTYPESTORE_LOOP(INIT)
  return ps;
}
