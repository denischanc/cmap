/*******************************************************************************
@TEST_DESC@ Call console info/error fns
*******************************************************************************/

#include "cmap-kernel.h"
#include "cmap-common.h"
#include "cmap-fw.h"

#include <stdlib.h>
#include "test-assert.h"

/*******************************************************************************
*******************************************************************************/

int main(int argc, char * argv[])
{
  cmap_kernel_create(NULL);

  CMAP_PROC_GLOBAL_ARGS("cmap.console.info",
    CMAP_STRING("Hello ", 0), CMAP_STRING("World ", 0), CMAP_STRING("!!!", 0));

  CMAP_PROC_GLOBAL("cmap.console.info");

  return cmap_kernel() -> main(argc, argv);
}
