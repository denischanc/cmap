
#include "cmap-kernel.h"

#include <stdlib.h>
#include "cmap-global-env.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL kernel_ = {0};
static CMAP_KERNEL * kernel_ptr_ = NULL;

/*******************************************************************************
*******************************************************************************/

static int kernel_main(int argc, char * argv[])
{
  cmap_kernel() -> exit(EXIT_SUCCESS);
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

static void kernel_exit(int ret)
{
  cmap_kernel_prototype_delete(&kernel_.prototype_);
  cmap_global_env_delete();

  exit(ret);
}

static void kernel_fatal()
{
  kernel_exit(EXIT_FAILURE);
}

/*******************************************************************************
*******************************************************************************/

static void upd_cfg(CMAP_KERNEL_CFG * cfg)
{
  if(cfg -> mem_ == NULL) cfg -> mem_ = cmap_mem_create(0);
}

/*******************************************************************************
*******************************************************************************/

void cmap_kernel_create(CMAP_KERNEL_CFG * cfg)
{
  CMAP_KERNEL_CFG cfg_dft = {0};
  if(cfg == NULL) cfg = &cfg_dft;
  upd_cfg(cfg);

  kernel_.mem_ = cfg -> mem_;
  kernel_.main = kernel_main;
  kernel_.exit = kernel_exit;
  kernel_.fatal = kernel_fatal;

  kernel_ptr_ = &kernel_;

  cmap_kernel_prototype_create(&kernel_.prototype_);
  cmap_global_env_create();
}

/*******************************************************************************
*******************************************************************************/

CMAP_KERNEL * cmap_kernel()
{
  return kernel_ptr_;
}
