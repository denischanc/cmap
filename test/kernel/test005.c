/*******************************************************************************
@TEST_DESC@ Free ptr 2 times
*******************************************************************************/

#include <stdlib.h>
#include "cmap-ext.h"
#include "cmap-mem.h"
#include "cmap-lifecycle.h"
#include "cmap-core.h"

/*******************************************************************************
*******************************************************************************/

static void test(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * tmp = cmap_map(proc_ctx);
  cmap_core_delete((CMAP_LIFECYCLE *)tmp, proc_ctx);
  cmap_mem_free(tmp);
}

int main(int argc, char * argv[])
{
  cmap_env_main(cmap_env(), test);
  return cmap_main();
}
