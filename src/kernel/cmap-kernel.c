
#include "cmap-kernel.h"

#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL kernel_;
static CMAP_KERNEL * kernel_ptr_ = NULL;

/*******************************************************************************
*******************************************************************************/

static int kernel_start(int argc, char * argv[])
{
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

static void kernel_exit(int ret)
{
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
  kernel_.start = kernel_start;
  kernel_.exit = kernel_exit;
  kernel_.fatal = kernel_fatal;

  kernel_ptr_ = &kernel_;
}

/*******************************************************************************
*******************************************************************************/

CMAP_KERNEL * cmap_kernel()
{
  return kernel_ptr_;
}
