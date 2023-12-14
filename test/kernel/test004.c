/*******************************************************************************
@TEST_DESC@ deepDeleteNoRef()
*******************************************************************************/

#include <stdlib.h>
#include "cmap-ext.h"

/*******************************************************************************
*******************************************************************************/

static void test(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * map1 = cmap_to_map(proc_ctx,
    "lien1", cmap_map(proc_ctx),
    "lien2", cmap_map(proc_ctx),
    "lien3", cmap_map(proc_ctx),
    NULL);

  cmap_to_map(proc_ctx,
    "lien1", map1,
    "lien2", cmap_map(proc_ctx),
    "lien3", cmap_map(proc_ctx),
    "lien4", cmap_map(proc_ctx),
    NULL);
}

int main(int argc, char * argv[])
{
  cmap_bootstrap(NULL);
  cmap_env_main(cmap_env(argc, argv), test);
  return cmap_main();
}
