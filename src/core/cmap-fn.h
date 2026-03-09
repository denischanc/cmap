#ifndef __CMAP_FN_H__
#define __CMAP_FN_H__

#include "cmap-fn-define.h"
#include "cmap-fn-ext.h"
#include "cmap-fn-int.h"
#include "cmap-map.h"
#include "cmap-list-type.h"

struct CMAP_FN
{
  CMAP_MAP super;

  CMAP_FN_INTERNAL internal;
};

void cmap_fn_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list,
  CMAP_PROC_CTX * proc_ctx);

CMAP_MAP * cmap_fn_process(CMAP_FN * fn, CMAP_PROC_CTX * proc_ctx,
  CMAP_MAP * map, CMAP_LIST * args);

CMAP_MAP * cmap_fn_new(CMAP_FN * fn, CMAP_LIST * args,
  CMAP_PROC_CTX * proc_ctx);

void cmap_fn_delete(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx);
CMAP_FN * cmap_fn_init(CMAP_FN * fn, CMAP_INITARGS * initargs,
  CMAP_FN_TPL process);
CMAP_FN * cmap_fn_create(CMAP_FN_TPL process, CMAP_PROC_CTX * proc_ctx);

#endif
