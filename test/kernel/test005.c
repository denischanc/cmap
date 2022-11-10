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

static CMAP_MAP * job_fn(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_MAP * tmp = $MAP(NULL);
  CMAP_DELETE(tmp);
  CMAP_KERNEL_FREE(tmp);

  return NULL;
}

int main(int argc, char * argv[])
{
  cmap_bootstrap(NULL);

  CMAP_FN * job_proto = (CMAP_FN *)$K_GET_G("cmap.scheduler.job");
  CMAP_MAP * job = $NEW_A(job_proto, CMAP_AISLE_GLOBAL,
    $FN(job_fn, CMAP_AISLE_GLOBAL));

  return cmap_main(argc, argv, job);
}
