
#include "cmap-kernel.h"

#include <stdlib.h>
#include "cmap-global-env.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL kernel_ = {0};
static CMAP_KERNEL * kernel_ptr_ = NULL;

static char exiting_ = CMAP_F;

/*******************************************************************************
*******************************************************************************/

static void create_all()
{
  kernel_.warehouse_ = cmap_warehouse_create();

  cmap_kernel_prototype_create();
  cmap_global_env_create();
}

/*******************************************************************************
*******************************************************************************/

static void delete_all()
{
  /* TODO */
  /*cmap_kernel_prototype_delete();*/
  /*cmap_global_env_delete();*/
  /* TODO */
  CMAP_CALL((CMAP_MAP *)kernel_.warehouse_, delete);
}

/*******************************************************************************
*******************************************************************************/

static void checkup(int * ret)
{
  kernel_.log_.debug("Allocated memory size : [%d].",
    cmap_mem_state() -> size_alloc_);
}

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
  if(!exiting_)
  {
    exiting_ = CMAP_T;

    delete_all();

    checkup(&ret);

    kernel_.log_.debug("Exit kernel (%d).", ret);
    exit(ret);
  }
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
  kernel_ptr_ = &kernel_;

  cmap_log_init();
  kernel_.log_.debug("Create kernel.");

  CMAP_KERNEL_CFG cfg_dft = {0};
  if(cfg == NULL) cfg = &cfg_dft;
  upd_cfg(cfg);

  kernel_.mem_ = cfg -> mem_;
  kernel_.main = kernel_main;
  kernel_.exit = kernel_exit;
  kernel_.fatal = kernel_fatal;

  create_all();
}

/*******************************************************************************
*******************************************************************************/

CMAP_KERNEL * cmap_kernel()
{
  return kernel_ptr_;
}
