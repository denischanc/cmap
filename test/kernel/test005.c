/*******************************************************************************
@TEST_DESC@ Free ptr 2 times
*******************************************************************************/

#include <stdlib.h>
#include "cmap-ext.h"
#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

static void test(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * tmp = cmap_map(proc_ctx, NULL);
  cmap_delete(tmp);
  CMAP_KERNEL_FREE(tmp);
}

int main(int argc, char * argv[])
{
  cmap_bootstrap(NULL);
  cmap_env_main(cmap_env(), argc, argv, test);
  return cmap_main();
}
