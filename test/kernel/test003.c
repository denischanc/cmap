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

static CMAP_MAP * main_(CMAP_MAP * features, CMAP_MAP * map, CMAP_LIST * args)
{
  $$$G(
    "cmap.console.info", L$($SL("Hello "), $SL("World "), $SL("!!!")),
    "info", $LL());

  $$G("cmap.console.info");

  return NULL;
}

int main(int argc, char * argv[])
{
  cmap_init(NULL);

  CMAP_FN * fn = CMAP_FN(main_, CMAP_AISLE_GLOBAL);
  CMAP_FN_PROC(fn, NULL);

  return cmap_main(argc, argv);
}
