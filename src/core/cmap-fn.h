#ifndef __CMAP_FN_H__
#define __CMAP_FN_H__

#include "cmap-fn-type.h"
#include "cmap-fn-define.h"
#include "cmap-fn-ext.h"
#include "cmap-map.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-list-type.h"
#include "cmap-lifecycle.h"

struct CMAP_FN
{
  CMAP_MAP super;

  void * internal;

  CMAP_MAP * (*require_definitions)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx);

  void (*add_arg_name)(CMAP_FN * this, const char * arg_name);

  CMAP_MAP * (*process)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
    CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*do_process)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
    CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*new)(CMAP_FN * this, CMAP_LIST * args,
    CMAP_PROC_CTX * proc_ctx, const char * aisle);
};

typedef struct {
  CMAP_FN * (*create)(CMAP_FN_TPL process, CMAP_PROC_CTX * proc_ctx,
    const char * aisle);
  void (*init)(CMAP_FN * fn, CMAP_FN_TPL process);
  CMAP_LIFECYCLE * (*delete)(CMAP_FN * fn);

  void (*add_arg_name)(CMAP_FN * fn, const char * arg_name);

  CMAP_MAP * (*process)(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx,
    CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*do_process)(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx,
    CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*new)(CMAP_FN * this, CMAP_LIST * args,
    CMAP_PROC_CTX * proc_ctx, const char * aisle);
} CMAP_FN_PUBLIC;

extern const CMAP_FN_PUBLIC cmap_fn_public;

#endif
