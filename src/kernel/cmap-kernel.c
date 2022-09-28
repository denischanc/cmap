
#include "cmap-kernel.h"

#include <stdlib.h>
#include "cmap-global-env.h"
#include "cmap-fw.h"
#include "cmap-aisle.h"
#include "cmap-util-list.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL kernel_ = {};
static CMAP_KERNEL * kernel_ptr_ = NULL;

static CMAP_KERNEL_CFG cfg_ = {};

static char exiting_ = CMAP_F;

static int state_ = CMAP_KERNEL_S_UNKNOWN;

/*******************************************************************************
*******************************************************************************/

static void init_fw_env()
{
  kernel_.fw_.warehouse_ = cmap_warehouse_create();

  cmap_prototype_init(&kernel_.fw_.prototype_);
  kernel_.fw_.pool_list_ = cmap_pool_list_create(20);
  kernel_.fw_.pool_string_ = cmap_pool_string_create(20);

  CMAP_LIST(0, CMAP_AISLE_STACK);

  kernel_.fw_.global_env_ = cmap_global_env_create();
}

/*******************************************************************************
*******************************************************************************/

static void delete_all()
{
  CMAP_WAREHOUSE * wh = kernel_.fw_.warehouse_;

  cmap_delete_list_vals((CMAP_LIST *)CMAP_GET(wh, CMAP_AISLE_STACK));
  CMAP_CALL(kernel_.fw_.pool_list_, delete);
  CMAP_CALL(kernel_.fw_.pool_string_, delete);
  CMAP_CALL((CMAP_MAP *)wh, delete);
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
  state_ = CMAP_KERNEL_S_EXITING;

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

static int kernel_state()
{
  return state_;
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
  state_ = CMAP_KERNEL_S_INIT;

  cmap_log_init(&kernel_.log_);
  kernel_.log_.debug("Init kernel.");

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
  kernel_.state = kernel_state;

  init_fw_env();

  state_ = CMAP_KERNEL_S_ALIVE;
}

/*******************************************************************************
*******************************************************************************/

CMAP_KERNEL * cmap_kernel()
{
  return kernel_ptr_;
}
