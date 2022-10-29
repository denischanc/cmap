/*******************************************************************************
@TEST_DESC@ Call console info/error fns
*******************************************************************************/

#include <cmap/cmap.h>

#include <stdlib.h>
#include "test-assert.h"

/*******************************************************************************
*******************************************************************************/

int main(int argc, char * argv[])
{
  cmap_init(NULL);

  CMAP_PROC_GLOBAL_ARGS("cmap.console.info",
    CMAP_STRING("Hello ", 0, cmap_aisle_public.local),
    CMAP_STRING("World ", 0, cmap_aisle_public.local),
    CMAP_STRING("!!!", 0, cmap_aisle_public.local));

  CMAP_PROC_GLOBAL("cmap.console.info");

  return cmap_main(argc, argv);
}
