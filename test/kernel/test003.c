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

static CMAP_MAP * test(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  return cmap$$(proc_ctx, NULL,
    "cmap.console.info(\"Hello \"<t>, \"World \"<t>, \"!!!\"<t>).info();"
    "cmap.console.info();");
}

int main(int argc, char * argv[])
{
  cmap_bootstrap(NULL);

  CMAP_ENV * env = cmap_env();
  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx(env);
  CMAP_MAP * definitions = CMAP_MAP(proc_ctx, CMAP_AISLE_GLOBAL);

  CMAP_SET(definitions, "test", CMAP_FN(test, proc_ctx, CMAP_AISLE_GLOBAL));
  cmap_env_main(env, argc, argv, definitions,
    "local job = new cmap.scheduler.job(test)<t>;"
    "job.schedule();");

  cmap_delete_proc_ctx(proc_ctx);

  return cmap_main();
}
