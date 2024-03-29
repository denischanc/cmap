#ifndef __CMAP_PTR_H__
#define __CMAP_PTR_H__

#include "cmap-ptr-type.h"
#include "cmap-ptr-ext.h"
#include "cmap-map.h"

struct CMAP_PTR
{
  CMAP_MAP super;

  void * internal;

  void * (*get)(CMAP_PTR * this);

  void ** (*ref)(CMAP_PTR * this);
};

typedef struct
{
  CMAP_PTR * (*create)(int size, CMAP_PTR_DELETE delete_ptr,
    CMAP_PROC_CTX * proc_ctx);
  CMAP_PTR * (*init)(CMAP_PTR * this, CMAP_INITARGS * initargs, int size,
    CMAP_PTR_DELETE delete_ptr);
  void (*delete)(CMAP_LIFECYCLE * this);

  void * (*get)(CMAP_PTR * this);

  void ** (*ref)(CMAP_PTR * this);
} CMAP_PTR_PUBLIC;

extern const CMAP_PTR_PUBLIC cmap_ptr_public;

#endif
