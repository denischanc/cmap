/*******************************************************************************
@TEST_DESC@ Zombie detection
*******************************************************************************/

#include <stdlib.h>
#include "test-assert.h"
#include "cmap.h"
#include "cmap-log.h"

static void test(CMAP_PROC_CTX * proc_ctx)
{
  CMAP_MAP * zombie = cmap_map(proc_ctx);
  CMAP_MAP * n12 = cmap_map(proc_ctx);
  CMAP_MAP * n13 = cmap_map(proc_ctx);
  CMAP_MAP * n14 = cmap_map(proc_ctx);
  CMAP_MAP * n15 = cmap_map(proc_ctx);

  CMAP_MAP * n21 = cmap_map(proc_ctx);
  CMAP_MAP * n22 = cmap_map(proc_ctx);
  CMAP_MAP * n23 = cmap_map(proc_ctx);
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

int main(int argc, char * argv[])
{
  cmap_dft_cfg() -> log_lvl = CMAP_LOG_DEBUG;
  cmap_env_main(cmap_env(argc, argv), test);
  return cmap_main();
}
