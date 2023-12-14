#ifndef __CMAP_POOL_H__
#define __CMAP_POOL_H__

#include "cmap-pool-define.h"
#include "cmap-list-type.h"
#include "cmap-string-type.h"
#include "cmap-int-type.h"

CMAP_POOL(LIST, list)
CMAP_POOL(STRING, string)
CMAP_POOL(INT, int)

#endif
