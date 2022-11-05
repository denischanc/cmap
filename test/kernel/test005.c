/*******************************************************************************
@TEST_DESC@ Free ptr 2 times
*******************************************************************************/

#include <cmap/cmap.h>
#define __CMAP_COMMON_H__
#include <cmap/aisle.h>
#include <cmap/define-min.h>
#include "cmap-kernel.h"

#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * main_(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_MAP * tmp = $MAP(NULL);
  CMAP_DELETE(tmp);
  CMAP_KERNEL_FREE(tmp);

  return NULL;
}

int main(int argc, char * argv[])
{
  cmap_init(NULL);

  CMAP_FN * fn = CMAP_FN(main_, CMAP_AISLE_GLOBAL);
  CMAP_FN_PROC(fn, NULL);

  return cmap_main(argc, argv);
}
