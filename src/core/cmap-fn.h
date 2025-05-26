#ifndef __CMAP_FN_H__
#define __CMAP_FN_H__

#include "cmap-fn-type.h"
#include "cmap-fn-define.h"
#include "cmap-fn-ext.h"
#include "cmap-map.h"
#include "cmap-list-type.h"

struct CMAP_FN
{
  CMAP_MAP super;

  void * internal;

  CMAP_MAP * (*require_definitions)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx);

  CMAP_MAP * (*process)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
    CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*do_process)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
    CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*new)(CMAP_FN * this, CMAP_LIST * args,
    CMAP_PROC_CTX * proc_ctx);
};

typedef struct {
  CMAP_FN * (*create)(CMAP_FN_TPL process, CMAP_PROC_CTX * proc_ctx);
  CMAP_FN * (*init)(CMAP_FN * this, CMAP_INITARGS * initargs,
    CMAP_FN_TPL process);
  void (*delete)(CMAP_LIFECYCLE * this);

  void (*nested)(CMAP_LIFECYCLE * this, CMAP_SLIST_LC_PTR * list);

  CMAP_MAP * (*require_definitions)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx);

  CMAP_MAP * (*process)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
    CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*do_process)(CMAP_FN * this, CMAP_PROC_CTX * proc_ctx,
    CMAP_MAP * map, CMAP_LIST * args);

  CMAP_MAP * (*new)(CMAP_FN * this, CMAP_LIST * args,
    CMAP_PROC_CTX * proc_ctx);
} CMAP_FN_PUBLIC;

extern const CMAP_FN_PUBLIC cmap_fn_public;

#endif
