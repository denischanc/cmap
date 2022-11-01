#ifndef __CMAP_PROTOTYPE_UTIL__
#define __CMAP_PROTOTYPE_UTIL__

#include "cmap-prototype-util-define.h"
#include "cmap-map.h"
#include "cmap-fn.h"
#include "cmap-list.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_MAP * map_;
  CMAP_FN * fn_;
} CMAP_PROTOTYPE_MAP_FN;

/*******************************************************************************
*******************************************************************************/

char cmap_prototype_args_map_fn(CMAP_PROTOTYPE_MAP_FN * map_fn,
  CMAP_LIST * args);

/*******************************************************************************
*******************************************************************************/

#endif
