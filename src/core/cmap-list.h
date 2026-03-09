#ifndef __CMAP_LIST_H__
#define __CMAP_LIST_H__

#include "cmap-list-define.h"
#include "cmap-list-ext.h"
#include "cmap-list-int.h"
#include "cmap-map.h"

typedef void (*CMAP_LIST_VAL_FN)(CMAP_MAP ** val, void * data,
  CMAP_PROC_CTX * proc_ctx);

struct CMAP_LIST
{
  CMAP_MAP super;

  CMAP_LIST_INTERNAL internal;
};

void cmap_list_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list,
  CMAP_PROC_CTX * proc_ctx);

int cmap_list_size(CMAP_LIST * list);

CMAP_LIST * cmap_list_add(CMAP_LIST * list, int i, CMAP_MAP * val);
CMAP_MAP * cmap_list_rm(CMAP_LIST * list, int i, CMAP_PROC_CTX * proc_ctx);

CMAP_LIST * cmap_list_push(CMAP_LIST * list, CMAP_MAP * val);
CMAP_MAP * cmap_list_pop(CMAP_LIST * list, CMAP_PROC_CTX * proc_ctx);

CMAP_LIST * cmap_list_unshift(CMAP_LIST * list, CMAP_MAP * val);
CMAP_MAP * cmap_list_shift(CMAP_LIST * list, CMAP_PROC_CTX * proc_ctx);

void cmap_list_apply(CMAP_LIST * list, CMAP_LIST_VAL_FN fn, void * data,
  CMAP_PROC_CTX * proc_ctx);

void cmap_list_clean(CMAP_LIST * list, CMAP_PROC_CTX * proc_ctx);

void cmap_list_delete(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx);
CMAP_LIST * cmap_list_init(CMAP_LIST * list, CMAP_INITARGS * initargs,
  int size_inc);
CMAP_LIST * cmap_list_create(int size_inc, CMAP_PROC_CTX * proc_ctx);

#endif
