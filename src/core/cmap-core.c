
#include "cmap-core.h"

#include "cmap-log.h"
#include "cmap-double.h"
#include "cmap-fn.h"
#include "cmap-int.h"
#include "cmap-list.h"
#include "cmap-map.h"
#include "cmap-ptr.h"
#include "cmap-string.h"

/*******************************************************************************
*******************************************************************************/

#define LOG_SIZEOF(NAME, name) \
  cmap_log(lvl, "sizeof 'CMAP_%s' : '%ld'", #NAME, sizeof(CMAP_##NAME));

void cmap_core_log_sizeof(char lvl)
{
  CMAP_CORE_LOOP(LOG_SIZEOF)
}
