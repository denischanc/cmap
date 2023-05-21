
#include "cmap-impl-fn.h"

#include <stdlib.h>
#include <string.h>
#include "cmap.h"
#include "cmap-list.h"
#include "cmap-util.h"
#include "cmap-kernel.h"
#include "cmap-mem.h"
#include "cmap-prototype-fn.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_LIST * arg_names;

  char * impl;
} INTERNAL;

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * do_process(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_MAP * definitions = cmap_definitions(proc_ctx);
  INTERNAL * internal = (INTERNAL *)((CMAP_IMPL_FN *)this) -> internal;

  int size = CMAP_CALL(internal -> arg_names, size);
  for(int i = 0; i < size; i++)
  {
    CMAP_STRING * arg_name =
      (CMAP_STRING *)cmap_list_get(internal -> arg_names, i);
    cmap_set(definitions, cmap_string_val(arg_name), cmap_list_get(args, i));
  }

  return cmap_proc_impl(internal -> impl, proc_ctx);
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * delete(CMAP_IMPL_FN * fn)
{
  INTERNAL * internal = (INTERNAL *)fn -> internal;
  cmap_util_public.delete_list_n_vals(internal -> arg_names);
  CMAP_KERNEL_FREE(internal -> impl);
  CMAP_KERNEL_FREE(internal);

  return cmap_fn_public.delete((CMAP_FN *)fn);
}

static CMAP_MAP * delete_(CMAP_MAP * fn)
{
  return delete((CMAP_IMPL_FN *)fn);
}

static void init(CMAP_IMPL_FN * fn, CMAP_LIST * arg_names, const char * impl,
  CMAP_PROC_CTX * proc_ctx)
{
  CMAP_FN * super = &fn -> super;
  cmap_fn_public.init(super, NULL);
  super -> do_process = do_process;

  (&super -> super) -> delete = delete_;

  CMAP_KERNEL_ALLOC_PTR(internal, INTERNAL);
  internal -> arg_names = cmap_util_public.dup_string(
    CMAP_LIST(0, proc_ctx, NULL), arg_names, proc_ctx, NULL);
  internal -> impl = cmap_util_public.strdup(impl);

  fn -> internal = internal;
}

static CMAP_IMPL_FN * create(CMAP_LIST * arg_names, const char * impl,
  CMAP_PROC_CTX * proc_ctx, const char * aisle)
{
  CMAP_MAP * prototype_fn = cmap_prototype_fn_public.instance(proc_ctx);
  CMAP_IMPL_FN * fn = (CMAP_IMPL_FN *)CMAP_CALL_ARGS(prototype_fn, new,
    sizeof(CMAP_IMPL_FN), proc_ctx, aisle);
  init(fn, arg_names, impl, proc_ctx);
  return fn;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_IMPL_FN_PUBLIC cmap_impl_fn_public =
{
  create,
  init,
  delete
};
