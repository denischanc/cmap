
#include "cmap-util-list.h"

#include <stdlib.h>
#include "cmap-list.h"
#include "cmap.h"

/*******************************************************************************
*******************************************************************************/

void cmap_delete_list_vals(CMAP_LIST * list)
{
  CMAP_MAP * val;
  while((val = CMAP_LIST_UNSHIFT(list)) != NULL) CMAP_DELETE(val);
}
