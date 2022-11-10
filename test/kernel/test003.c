/*******************************************************************************
@TEST_DESC@ Call console info/error fns
*******************************************************************************/

#include <cmap/cmap.h>
#include <cmap/aisle.h>
#include <cmap/define-min.h>

#include <stdlib.h>
#include "test-assert.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * job_fn(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  $G_$$(
    "cmap.console.info",
      $$LIST_L($STR_L("Hello "), $STR_L("World "), $STR_L("!!!")),
    "info", $LIST_L());

  $G_$("cmap.console.info");

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
