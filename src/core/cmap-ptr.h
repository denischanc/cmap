#ifndef __CMAP_PTR_H__
#define __CMAP_PTR_H__

#include "cmap-ptr-type.h"
#include "cmap-ptr-ext.h"
#include "cmap-map.h"

struct CMAP_PTR_s
{
  CMAP_MAP super;

  void * internal;

  void * (*get)(CMAP_PTR * this);
};

typedef struct
{
  CMAP_PTR * (*create)(int size, CMAP_PROC_CTX * proc_ctx, const char * aisle);
  void (*init)(CMAP_PTR * ptr, int size);
  CMAP_MAP * (*delete)(CMAP_PTR * ptr);

  void * (*get)(CMAP_PTR * this);
} CMAP_PTR_PUBLIC;

extern const CMAP_PTR_PUBLIC cmap_ptr_public;

#endif
