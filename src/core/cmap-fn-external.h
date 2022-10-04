#ifndef __CMAP_FN_EXTERNAL_H__
#define __CMAP_FN_EXTERNAL_H__

#include "cmap-core.h"

typedef CMAP_MAP * (*CMAP_FN_TPL)(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args);

#endif
