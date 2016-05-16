
#include "cmap-kernel-prototype.h"

#include <stdlib.h>
#include "cmap-common.h"
#include "cmap-fw.h"

/*******************************************************************************
*******************************************************************************/

#define CMAP_KERNEL_PROTOTYPE_APPLY(macro, prototype_s) \
  macro(map, prototype_s) \
  macro(list, prototype_s) \
  macro(fn, prototype_s) \
  macro(string, prototype_s) \
  macro(int, prototype_s)

#define CMAP_KERNEL_PROTOTYPE_CREATE(name, prototype_s) \
  prototype_s -> name##_ = CMAP_MAP("TODO");

#define CMAP_KERNEL_PROTOTYPE_DELETE(name, prototype_s) \
  CMAP_MAP * prototype_##name = prototype_s -> name##_; \
  if(prototype_##name != NULL) \
  { \
    CMAP_CALL(prototype_##name, delete); \
    prototype_s -> name##_ = NULL; \
  }

/*******************************************************************************
*******************************************************************************/

void cmap_kernel_prototype_create()
{
  CMAP_KERNEL_PROTOTYPE * prototype = &cmap_kernel() -> prototype_;
  CMAP_KERNEL_PROTOTYPE_APPLY(CMAP_KERNEL_PROTOTYPE_CREATE, prototype)
}

/*******************************************************************************
*******************************************************************************/

void cmap_kernel_prototype_delete()
{
  CMAP_KERNEL_PROTOTYPE * prototype = &cmap_kernel() -> prototype_;
  CMAP_KERNEL_PROTOTYPE_APPLY(CMAP_KERNEL_PROTOTYPE_DELETE, prototype)
}
