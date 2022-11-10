
#include "cmap-ctx.h"

#include "cmap-kernel.h"
#include <stdlib.h>
#include "cmap-global-env.h"

/*******************************************************************************
*******************************************************************************/

#define CTX_LOOP(macro) \
  macro(CMAP_AISLESTORE, aislestore) \
  macro(CMAP_POOL_LIST, pool_list, 20) \
  macro(CMAP_POOL_STRING, pool_string, 20) \
  macro(CMAP_MAP, global_env)

/*******************************************************************************
*******************************************************************************/

#define DECLARE(struct, name, args...) \
  struct * name;

typedef struct
{
  CTX_LOOP(DECLARE)
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

#define GETTER(struct, name, args...) \
static struct * name(CMAP_CTX * this) \
{ \
  INTERNAL * internal = (INTERNAL *)this -> internal; \
  if(internal -> name == NULL) \
  { \
    internal -> name = cmap_##name##_public.create(args); \
  } \
  return internal -> name; \
}

CTX_LOOP(GETTER)

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_CTX * this)
{
  CMAP_CALL(pool_list(this), delete);
  CMAP_CALL(pool_string(this), delete);

  CMAP_MAP * as = (CMAP_MAP *)aislestore(this);
  CMAP_CALL(as, delete);

  CMAP_KERNEL_FREE(this -> internal);
  CMAP_KERNEL_FREE(this);
}

#define INIT(struct, name, args...) \
  internal -> name = NULL; \
  ctx -> name = name;

static CMAP_CTX * create()
{
  CMAP_KERNEL_ALLOC_PTR(ctx, CMAP_CTX);
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);

  CTX_LOOP(INIT)
  ctx -> internal = internal;
  ctx -> delete = delete;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CTX_PUBLIC cmap_ctx_public =
{
  create
};
