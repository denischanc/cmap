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

  CMAP_MAP * cmap = CMAP_CALL_ARGS(cmap_kernel() -> global_env_, get, "cmap");
  CMAP_MAP * console = CMAP_CALL_ARGS(cmap, get, "console");
  CMAP_FN * info = (CMAP_FN *)CMAP_CALL_ARGS(console, get, "info");

  CMAP_LIST * args = _L_(0);
  CMAP_CALL_ARGS(args, push, (CMAP_MAP *)_S_("Hello ", 0));
  CMAP_CALL_ARGS(args, push, (CMAP_MAP *)_S_("World ", 0));
  CMAP_CALL_ARGS(args, push, (CMAP_MAP *)_S_("!!!", 0));

  CMAP_CALL_ARGS(info, process, console, args);

  CMAP_CALL_ARGS(info, process, console, NULL);

  return cmap_kernel() -> main(argc, argv);
}
