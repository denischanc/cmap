#ifndef __CMAP_LIST_H__
#define __CMAP_LIST_H__

#include "cmap-list-type.h"
#include "cmap-list-define.h"
#include "cmap-list-ext.h"
#include "cmap-map.h"

typedef void (*CMAP_LIST_VAL_FN)(CMAP_MAP ** val, void * data);

struct CMAP_LIST
{
  CMAP_MAP super;

  void * internal;

  int (*size)(CMAP_LIST * this);

  CMAP_LIST * (*set)(CMAP_LIST * this, int i, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_LIST * this, int i);

  CMAP_LIST * (*add)(CMAP_LIST * this, int i, CMAP_MAP * val);
  CMAP_MAP * (*rm)(CMAP_LIST * this, int i);

  CMAP_LIST * (*push)(CMAP_LIST * this, CMAP_MAP * val);
  CMAP_MAP * (*pop)(CMAP_LIST * this);

  CMAP_LIST * (*unshift)(CMAP_LIST * this, CMAP_MAP * val);
  CMAP_MAP * (*shift)(CMAP_LIST * this);

  void (*apply)(CMAP_LIST * this, CMAP_LIST_VAL_FN fn, void * data);

  void (*clean)(CMAP_LIST * this);
};

typedef struct
{
  CMAP_LIST * (*create)(int size_inc, CMAP_PROC_CTX * proc_ctx);
  void (*init)(CMAP_LIST * list, int size_inc);
  void (*delete)(CMAP_LIFECYCLE * this);

  void (*nested)(CMAP_LIFECYCLE * this, CMAP_STACK_lc_ptr ** stack);

  int (*size)(CMAP_LIST * this);

  CMAP_LIST * (*set)(CMAP_LIST * this, int i, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_LIST * this, int i);

  CMAP_LIST * (*add)(CMAP_LIST * this, int i, CMAP_MAP * val);
  CMAP_MAP * (*rm)(CMAP_LIST * this, int i);

  CMAP_LIST * (*push)(CMAP_LIST * this, CMAP_MAP * val);
  CMAP_MAP * (*pop)(CMAP_LIST * this);

  CMAP_LIST * (*unshift)(CMAP_LIST * this, CMAP_MAP * val);
  CMAP_MAP * (*shift)(CMAP_LIST * this);

  void (*apply)(CMAP_LIST * this, CMAP_LIST_VAL_FN fn, void * data);

  void (*clean)(CMAP_LIST * this);
} CMAP_LIST_PUBLIC;

extern const CMAP_LIST_PUBLIC cmap_list_public;

#endif
