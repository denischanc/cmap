/*******************************************************************************
@TEST_DESC@ deepDeleteNoRef()
*******************************************************************************/

#include <stdlib.h>
#include "cmap-ext.h"
#include "cmap-aisle-ext.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * test(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
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

  cmap_proc(map2, "deepDeleteNoRef", proc_ctx);

  return NULL;
}

int main(int argc, char * argv[])
{
  cmap_bootstrap(NULL);

  CMAP_ENV * env = cmap_env();

  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx(env);
  CMAP_MAP * definitions = cmap_map(proc_ctx, CMAP_AISLE_GLOBAL);
  CMAP_FN * test_fn = cmap_fn(test, proc_ctx, CMAP_AISLE_GLOBAL);
  cmap_delete_proc_ctx(proc_ctx);

  cmap_set(definitions, "test", (CMAP_MAP *)test_fn);
  cmap_env_main(env, argc, argv, definitions, "test();");

  return cmap_main();
}
