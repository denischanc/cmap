/*******************************************************************************
@TEST_DESC@ Zombie detection
*******************************************************************************/

#include <stdlib.h>
#include "test-assert.h"
#include "cmap.h"
#include "cmap-log.h"
#include "cmap-refswatcher.h"
#include "cmap-lifecycle.h"

static CMAP_MAP * zombie;
static CMAP_MAP * n15;
static CMAP_MAP * n21;
static CMAP_MAP * n23;

static void init(CMAP_PROC_CTX * proc_ctx)
{
  zombie = cmap_map(proc_ctx);
  CMAP_MAP * n12 = cmap_map(proc_ctx);
  CMAP_MAP * n13 = cmap_map(proc_ctx);
  CMAP_MAP * n14 = cmap_map(proc_ctx);
  n15 = cmap_map(proc_ctx);

  n21 = cmap_map(proc_ctx);
  CMAP_MAP * n22 = cmap_map(proc_ctx);
  n23 = cmap_map(proc_ctx);
  CMAP_MAP * n24 = cmap_map(proc_ctx);
  CMAP_MAP * n25 = cmap_map(proc_ctx);

  cmap_set(zombie, "n12", n12);
  cmap_set(zombie, "n13", n13);
  cmap_set(zombie, "n22", n22);
  cmap_set(zombie, "n25", n25);
  cmap_set(zombie, "n31", cmap_map(proc_ctx));

  cmap_set(n12, "n14", n14);
  cmap_set(n12, "n15", n15);

  cmap_set(n13, "n14", n14);

  cmap_set(n14, "n11", zombie);

  cmap_set(n15, "n12", n12);

  cmap_set(n21, "n22", n22);
  cmap_set(cmap_global_env(proc_ctx), "n21", n21);

  cmap_set(n22, "n23", n23);

  cmap_set(n23, "n24", n24);

  cmap_set(n24, "n22", n22);
  cmap_set(n24, "n25", n25);

  cmap_set(n25, "n26", cmap_map(proc_ctx));
}

static void test_map(CMAP_MAP * map, CMAP_REFSWATCHER * refswatcher,
  const char * desc, char ok)
{
  CMAP_LIFECYCLE * lc = (CMAP_LIFECYCLE *)map;

  CMAP_CALL_ARGS(refswatcher, add, lc);

  char ret = CMAP_CALL_ARGS(refswatcher, is_zombie, lc);
  CMAP_TEST_ASSERT((ok) ? ret : !ret, desc);

  CMAP_CALL_ARGS(refswatcher, rm, lc);
}

static void test(CMAP_ENV * env)
{
  CMAP_PROC_CTX * proc_ctx = cmap_proc_ctx(env);

  CMAP_REFSWATCHER * refswatcher = cmap_refswatcher_public.create();

  test_map(zombie, refswatcher, "zombie is a zombie", CMAP_T);
  test_map(n15, refswatcher, "n15 is a zombie", CMAP_T);

  test_map(n21, refswatcher, "n21 is not a zombie", CMAP_F);
  test_map(n23, refswatcher, "n23 is not a zombie", CMAP_F);

  CMAP_CALL_ARGS(refswatcher, add, (CMAP_LIFECYCLE *)zombie);
  CMAP_CALL_ARGS(refswatcher, delete_if_zombie, (CMAP_LIFECYCLE *)zombie);

  CMAP_DELETE(refswatcher);

  cmap_delete_proc_ctx(proc_ctx, NULL);
}

int main(int argc, char * argv[])
{
  cmap_dft_cfg() -> log_lvl = CMAP_LOG_DEBUG;
  CMAP_ENV * env = cmap_env(argc, argv);
  cmap_env_main(env, init);
  test(env);
  return cmap_main();
}
