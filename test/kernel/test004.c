/*******************************************************************************
@TEST_DESC@ Test deepDeleteNoRef()
*******************************************************************************/

#include <cmap/cmap.h>
#include <cmap/aisle.h>
#include <cmap/define-min.h>

#include <stdlib.h>
#include "test-assert.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * main_(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
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
  cmap_init(NULL);

  CMAP_FN * fn = CMAP_FN(main_, CMAP_AISLE_GLOBAL);
  CMAP_FN_PROC(fn, NULL);

  return cmap_main(argc, argv);
}
