
#include "cmap-util-list.h"

#include <stdlib.h>
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

void cmap_delete_list_vals(CMAP_LIST * list)
{
  CMAP_MAP * val;
  while((val = CMAP_UNSHIFT(list)) != NULL) CMAP_DELETE(val);
}
