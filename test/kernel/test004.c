/*******************************************************************************
@TEST_DESC@ deepDeleteNoRef()
*******************************************************************************/

#include <stdlib.h>
#include "cmap-ext.h"
#include "cmap-aisle-ext.h"

/*******************************************************************************
*******************************************************************************/

static void test(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * map1 = cmap_to_map(proc_ctx, NULL,
    "lien1", cmap_map(proc_ctx, NULL),
    "lien2", cmap_map(proc_ctx, CMAP_AISLE_LOCAL),
    "lien3", cmap_map(proc_ctx, "aisle1"),
    NULL);

  CMAP_MAP * map2 = cmap_to_map(proc_ctx, NULL,
    "lien1", map1,
    "lien2", cmap_map(proc_ctx, CMAP_AISLE_LOCAL),
    "lien3", cmap_map(proc_ctx, "aisle1"),
    "lien4", cmap_map(proc_ctx, NULL),
    NULL);

  cmap_proc(map2, "deepDeleteNoRef", proc_ctx, NULL);
}

int main(int argc, char * argv[])
{
  cmap_bootstrap(NULL);
  cmap_env_main(cmap_env(), argc, argv, test);
  return cmap_main();
}
