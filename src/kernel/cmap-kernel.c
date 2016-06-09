
#include "cmap-kernel.h"

#include <stdlib.h>
#include "cmap-global-env.h"
#include "cmap-common.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL kernel_ = {0};
static CMAP_KERNEL * kernel_ptr_ = NULL;

static CMAP_KERNEL_CFG cfg_ = {0};

static char exiting_ = CMAP_F;

/*******************************************************************************
*******************************************************************************/

static void create_or_init_others()
{
  kernel_.warehouse_ = cmap_warehouse_create();

  cmap_prototype_init(&kernel_.prototype_);
  kernel_.global_env_ = cmap_global_env_create();
}

/*******************************************************************************
*******************************************************************************/

static void delete_all()
{
  CMAP_CALL((CMAP_MAP *)kernel_.warehouse_, delete);
}

/*******************************************************************************
*******************************************************************************/

static void check_mem(int * ret)
{
  if(cmap_mem_is_this(kernel_.mem_))
  {
    int s = cmap_mem_state() -> size_alloc_;
    kernel_.log_.debug("Allocated memory size : [%d].", s);
    if((s != 0) && kernel_.cfg_ -> failure_on_allocmem_) *ret = EXIT_FAILURE;
  }
}

static void check_all(int * ret)
{
  check_mem(ret);
}

/*******************************************************************************
*******************************************************************************/

static void kernel_exit(int ret)
{
  if(!exiting_)
  {
    exiting_ = CMAP_T;

    delete_all();

    check_all(&ret);

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

static int kernel_main(int argc, char * argv[])
{
  kernel_exit(EXIT_SUCCESS);
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

static void cfg_init(CMAP_KERNEL_CFG * cfg)
{
  cfg -> mem_ = NULL;
  cfg -> failure_on_allocmem_ = CMAP_T;
}

/*******************************************************************************
*******************************************************************************/

inline CMAP_MEM * get_mem(CMAP_KERNEL_CFG * cfg)
{
  CMAP_MEM * mem = cfg -> mem_;
  if(mem == NULL) mem = cmap_mem_create(0);
  return mem;
}

/*******************************************************************************
*******************************************************************************/

void cmap_kernel_init(CMAP_KERNEL_CFG * cfg)
{
  kernel_ptr_ = &kernel_;

  cmap_log_init(&kernel_.log_);
  kernel_.log_.debug("Create kernel.");

  if(cfg == NULL)
  {
    cfg = &cfg_;
    cfg_init(cfg);
  }
  kernel_.cfg_ = cfg;

  kernel_.mem_ = get_mem(cfg);
  kernel_.main = kernel_main;
  kernel_.exit = kernel_exit;
  kernel_.fatal = kernel_fatal;

  create_or_init_others();
}

/*******************************************************************************
*******************************************************************************/

CMAP_KERNEL * cmap_kernel()
{
  return kernel_ptr_;
}
