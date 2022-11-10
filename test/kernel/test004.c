/*******************************************************************************
@TEST_DESC@ deepDeleteNoRef()
*******************************************************************************/

#include <cmap/cmap.h>
#include <cmap/aisle.h>
#include <cmap/define-min.h>

#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * job_fn(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  CMAP_MAP * map1 = $MAP(NULL);
  $SET(map1, "lien1", $MAP(NULL));
  $SET(map1, "lien2", $MAP_L());
  $SET(map1, "lien3", $MAP("aisle1"));

  CMAP_MAP * map2 = $MAP(NULL);
  $SET(map2, "lien1", map1);
  $SET(map2, "lien2", $MAP_L());
  $SET(map2, "lien3", $MAP("aisle1"));
  $SET(map2, "lien4", $MAP(NULL));

  $$(map2, "deepDeleteNoRef");

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
