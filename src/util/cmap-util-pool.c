
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
  CMAP_KERNEL * kernel = cmap_kernel_public.instance();
  CMAP_POOL_STRING * pool_string = kernel -> pool_string;
  CMAP_POOL_LIST * pool_list = kernel -> pool_list;

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
