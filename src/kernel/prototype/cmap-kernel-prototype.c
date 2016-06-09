
#include "cmap-kernel-prototype.h"

#include <stdlib.h>
#include "cmap-fw.h"
#include "cmap-aisle.h"

/*******************************************************************************
*******************************************************************************/

#define CMAP_KERNEL_PROTOTYPE_APPLY(macro, prototype_s) \
  macro(map, prototype_s) \
  macro(list, prototype_s) \
  macro(fn, prototype_s) \
  macro(string, prototype_s) \
  macro(int, prototype_s)

#define CMAP_KERNEL_PROTOTYPE_CREATE(name, prototype_s) \
  prototype_s -> name##_ = CMAP_MAP(CMAP_AISLE_KERNEL);

/*******************************************************************************
*******************************************************************************/

void cmap_prototype_init(CMAP_KERNEL_PROTOTYPE * prototype)
{
  CMAP_KERNEL_PROTOTYPE_APPLY(CMAP_KERNEL_PROTOTYPE_CREATE, prototype)
}
