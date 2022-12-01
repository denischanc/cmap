#ifndef __CMAP_FN_H__
#define __CMAP_FN_H__

#include <cmap/core.h>
#include "cmap-map.h"
#include <cmap/fn.h>
#include <cmap/nature.h>
#include "cmap-proc-ctx.h"

struct CMAP_FN_s
{
  CMAP_MAP super;

  void * internal;

  CMAP_MAP * (*require_definitions)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx);
  CMAP_MAP * (*definitions)(CMAP_FN * this);

  CMAP_MAP * (*process)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
    CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*new)(CMAP_FN * this, CMAP_LIST * args,
    CMAP_PROC_CTX * proc_ctx, const char * aisle);
};

typedef struct {
  CMAP_FN * (*create)(CMAP_FN_TPL process, CMAP_PROC_CTX * proc_ctx,
    const char * aisle);
  void (*init)(CMAP_FN * fn, CMAP_FN_TPL process);
  CMAP_MAP * (*delete)(CMAP_FN * fn);

  CMAP_MAP * (*process)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
    CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*new)(CMAP_FN * this, CMAP_LIST * args,
    CMAP_PROC_CTX * proc_ctx, const char * aisle);
} CMAP_FN_PUBLIC;

extern const CMAP_FN_PUBLIC cmap_fn_public;

#endif
