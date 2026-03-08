#ifndef __CMAP_PTR_H__
#define __CMAP_PTR_H__

#include "cmap-ptr-ext.h"
#include "cmap-ptr-int.h"
#include "cmap-map.h"

struct CMAP_PTR
{
  CMAP_MAP super;

  CMAP_PTR_INTERNAL internal;
};

void cmap_ptr_delete(CMAP_LIFECYCLE * lc);
CMAP_PTR * cmap_ptr_init(CMAP_PTR * ptr, CMAP_INITARGS * initargs, int size,
  CMAP_PTR_DELETE delete_ptr);
CMAP_PTR * cmap_ptr_create(int size, CMAP_PTR_DELETE delete_ptr,
  CMAP_PROC_CTX * proc_ctx);

#endif
