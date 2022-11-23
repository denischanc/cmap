/*******************************************************************************
@TEST_DESC@ deepDeleteNoRef()
*******************************************************************************/

#include <cmap/cmap.h>
#include <cmap/aisle.h>
#include <cmap/define-min.h>

#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * test(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_MAP * map1 = $MAP(proc_ctx, NULL);
  $SET(map1, "lien1", $MAP(proc_ctx, NULL));
  $SET(map1, "lien2", $MAP_L(proc_ctx));
  $SET(map1, "lien3", $MAP(proc_ctx, "aisle1"));

  CMAP_MAP * map2 = $MAP(proc_ctx, NULL);
  $SET(map2, "lien1", map1);
  $SET(map2, "lien2", $MAP_L(proc_ctx));
  $SET(map2, "lien3", $MAP(proc_ctx, "aisle1"));
  $SET(map2, "lien4", $MAP(proc_ctx, NULL));

  $$(map2, "deepDeleteNoRef", proc_ctx);

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
