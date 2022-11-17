
#include "cmap-ctx.h"

#include "cmap-kernel.h"
#include <stdlib.h>
#include "cmap-global-env.h"
#include "cmap-util.h"

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

  uv_loop_t * uv_loop;
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

static uv_loop_t * uv_loop(CMAP_CTX * this)
{
  INTERNAL * internal = (INTERNAL *)this -> internal;
  if(internal -> uv_loop == NULL)
  {
    internal -> uv_loop = CMAP_KERNEL_ALLOC(uv_loop_t);
    cmap_util_public.uv_error(uv_loop_init(internal -> uv_loop));
  }
  return internal -> uv_loop;
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_CTX * this)
{
  CMAP_CALL(pool_list(this), delete);
  CMAP_CALL(pool_string(this), delete);

  CMAP_MAP * as = (CMAP_MAP *)aislestore(this);
  CMAP_CALL(as, delete);

  CMAP_KERNEL_FREE(uv_loop(this));

  CMAP_KERNEL_FREE(this -> internal);
  CMAP_KERNEL_FREE(this);
}

#define INIT_INTERNAL(struct, name, args...) internal -> name = NULL;
#define INIT_CTX(struct, name, args...) ctx -> name = name;

static CMAP_CTX * create()
{
  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  CTX_LOOP(INIT_INTERNAL)
  internal -> uv_loop = NULL;

  CMAP_KERNEL_ALLOC_PTR(ctx, CMAP_CTX);
  ctx -> internal = internal;
  ctx -> delete = delete;
  CTX_LOOP(INIT_CTX)
  ctx -> uv_loop = uv_loop;

  return ctx;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CTX_PUBLIC cmap_ctx_public =
{
  create
};
