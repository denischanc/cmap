
#include "cmap-array.h"

/*******************************************************************************
*******************************************************************************/

const char * CMAP_ARRAY_NATURE = "cmap.nature.array";

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  int size_, max_;
  CMAP_MAP ** list_;
} CMAP_INTERNAL;

/*******************************************************************************
*******************************************************************************/
