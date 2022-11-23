/*******************************************************************************
@TEST_DESC@ Free ptr 2 times
*******************************************************************************/

#include <cmap/cmap.h>
#define __CMAP_COMMON_H__
#include <cmap/aisle.h>
#include <cmap/define-min.h>
#include "cmap-kernel.h"

#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * test(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_MAP * tmp = $MAP(proc_ctx, NULL);
  CMAP_DELETE(tmp);
  CMAP_KERNEL_FREE(tmp);

  return NULL;
}

int main(int argc, char * argv[])
{
  cmap_bootstrap(NULL);

  CMAP_ENV * env = cmap_env();
  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx(env);
  CMAP_MAP * definitions = CMAP_MAP(proc_ctx, NULL);
  CMAP_FN * test_fn = CMAP_FN(test, proc_ctx, NULL);

  CMAP_SET(definitions, "test", test_fn);
  cmap_env_main(env, argc, argv, definitions, "test();");

  CMAP_DELETE(test_fn);
  CMAP_DELETE(definitions);
  cmap_delete_proc_ctx(proc_ctx);

  return cmap_main();
}
