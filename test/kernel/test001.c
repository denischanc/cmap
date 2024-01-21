/*******************************************************************************
@TEST_DESC@ Instantiate kernel
*******************************************************************************/

#include <stdlib.h>
#include "cmap-ext.h"

static void empty(CMAP_PROC_CTX * proc_ctx)
{
}

int main(int argc, char * argv[])
{
  cmap_env_main(cmap_env(argc, argv), empty);
  return cmap_main();
}
