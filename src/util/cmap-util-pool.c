
#include "cmap-util-pool.h"

#include <stdlib.h>
#include "cmap-kernel.h"
#include "cmap-pool.h"
#include "cmap-common-define.h"
#include "cmap.h"

/*******************************************************************************
*******************************************************************************/

void cmap_release_list_n_strings(CMAP_LIST * list)
{
  CMAP_KERNEL_FW * fw = &(cmap_kernel() -> fw_);
  CMAP_POOL_STRING * pool_string = fw -> pool_string_;
  CMAP_POOL_LIST * pool_list = fw -> pool_list_;

  CMAP_MAP * val;
  while((val = CMAP_LIST_POP(list)) != NULL)
  {
    if(CMAP_NATURE(val) == cmap_string_public.nature)
    {
      CMAP_CALL_ARGS(pool_string, release, (CMAP_STRING *)val);
    }
  }

  CMAP_CALL_ARGS(pool_list, release, list);
}
