#ifndef __CMAP_IMPL_FN_H__
#define __CMAP_IMPL_FN_H__

#include "cmap-fn.h"
#include "cmap-list-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-lifecycle.h"

typedef struct
{
  CMAP_FN super;

  void * internal;
} CMAP_IMPL_FN;

typedef struct {
  CMAP_IMPL_FN * (*create)(CMAP_LIST * arg_names, const char * impl,
    CMAP_PROC_CTX * proc_ctx, const char * aisle);
  void (*init)(CMAP_IMPL_FN * fn, CMAP_LIST * arg_names, const char * impl,
    CMAP_PROC_CTX * proc_ctx);
  CMAP_LIFECYCLE * (*delete)(CMAP_IMPL_FN * fn);
} CMAP_IMPL_FN_PUBLIC;

extern const CMAP_IMPL_FN_PUBLIC cmap_impl_fn_public;

#endif
