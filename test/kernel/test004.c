/*******************************************************************************
@TEST_DESC@ deepDeleteNoRef()
*******************************************************************************/

#include <cmap/cmap.h>
#include <cmap/aisle.h>
#include <cmap/define-min.h>

#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * test(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
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

  CMAP_MAP * definitions = CMAP_MAP(CMAP_AISLE_GLOBAL);
  CMAP_SET(definitions, "test", CMAP_FN(test, CMAP_AISLE_GLOBAL));

  return cmap_main(argc, argv, definitions, "test();");
}
