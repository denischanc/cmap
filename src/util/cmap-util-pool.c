
#include "cmap-util-pool.h"

#include "cmap-kernel.h"
#include "cmap-pool.h"
#include "cmap-common.h"
#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

void cmap_release_list_n_strings(CMAP_LIST * list)
{
  CMAP_KERNEL_FW * fw = &(cmap_kernel() -> fw_);
  CMAP_POOL_STRING * pool_string = fw -> pool_string_;
  CMAP_POOL_LIST * pool_list = fw -> pool_list_;

  CMAP_MAP * val;
  while((val = CMAP_POP(list)) != NULL)
  {
    if(CMAP_NATURE(val) == CMAP_STRING_NATURE)
    {
      CMAP_CALL_ARGS(pool_string, release, (CMAP_STRING *)val);
    }
  }

  CMAP_CALL_ARGS(pool_list, release, list);
}
