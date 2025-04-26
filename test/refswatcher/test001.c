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
  cmap_set(zombie, "n31", cmap_map(proc_ctx));

  cmap_set(n12, "n14", n14);
  cmap_set(n12, "n15", n15);

  cmap_set(n13, "n14", n14);

  cmap_set(n14, "n11", zombie);

  cmap_set(n15, "n12", n12);

  cmap_set(n21, "n22", n22);

  cmap_set(n22, "n23", n23);

  cmap_set(n23, "n24", n24);

  cmap_set(n24, "n22", n22);
  cmap_set(n24, "n25", n25);

  cmap_set(n25, "n26", cmap_map(proc_ctx));

  CMAP_INC_REFS(n21); CMAP_INC_REFS(n21);
}

void test()
{
  CMAP_REFSWATCHER * refswatcher = cmap_refswatcher_public.create();

  CMAP_TEST_ASSERT(
    CMAP_CALL_ARGS(refswatcher, is_zombie, (CMAP_LIFECYCLE *)zombie),
    "zombie is a zombie");
  CMAP_TEST_ASSERT(
    CMAP_CALL_ARGS(refswatcher, is_zombie, (CMAP_LIFECYCLE *)n15),
    "n15 is a zombie");

  CMAP_TEST_ASSERT(
    !CMAP_CALL_ARGS(refswatcher, is_zombie, (CMAP_LIFECYCLE *)n21),
    "n21 is not a zombie");
  CMAP_TEST_ASSERT(
    !CMAP_CALL_ARGS(refswatcher, is_zombie, (CMAP_LIFECYCLE *)n23),
    "n23 is not a zombie");

  CMAP_DELETE(refswatcher);
}

int main(int argc, char * argv[])
{
  cmap_dft_cfg() -> log_lvl = CMAP_LOG_DEBUG;
  cmap_env_main(cmap_env(argc, argv), init);
  test();
  return cmap_main();
}
