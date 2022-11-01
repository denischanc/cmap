
#include "cmap-prototype.h"

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

#define INIT(proto) cmap_prototype_##proto##_public.init();

static void init()
{
  LOOP(INIT)
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PROTOTYPE_PUBLIC cmap_prototype_public = { init };
