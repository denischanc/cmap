
#include "cmap-prototype.h"

#include <stdlib.h>
#include "cmap-fw.h"
#include "cmap-aisle.h"
#include "cmap-prototype-map.h"
#include "cmap-prototype-list.h"
#include "cmap-prototype-fn.h"
#include "cmap-prototype-string.h"
#include "cmap-prototype-int.h"

/*******************************************************************************
*******************************************************************************/

#define LOOP(macro) \
  macro(map) \
  macro(list) \
  macro(fn) \
  macro(string) \
  macro(int)

/*******************************************************************************
*******************************************************************************/

#define CREATE(proto) \
  prototype -> proto##_ = CMAP_MAP(CMAP_AISLE_KERNEL);

#define INIT(proto) \
  cmap_prototype_##proto##_init(prototype -> proto##_);

void cmap_prototype_init(CMAP_PROTOTYPE * prototype)
{
  LOOP(CREATE)
  LOOP(INIT)
}
