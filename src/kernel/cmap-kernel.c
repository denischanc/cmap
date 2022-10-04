
#include "cmap-kernel.h"

#include <stdlib.h>
#include "cmap-global-env.h"
#include "cmap.h"
#include "cmap-aisle.h"
#include "cmap-util-list.h"

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL kernel = {};
static CMAP_KERNEL * kernel_ptr = NULL;

static CMAP_KERNEL_CFG cfg = {};

static char exiting = CMAP_F;

static int state = CMAP_KERNEL_S_UNKNOWN;

/*******************************************************************************
*******************************************************************************/

static void init_fw_env()
{
  kernel.fw_.warehouse_ = cmap_warehouse_create();

  cmap_prototype_init(&kernel.fw_.prototype_);
  kernel.fw_.pool_list_ = cmap_pool_list_create(20);
  kernel.fw_.pool_string_ = cmap_pool_string_create(20);

  CMAP_LIST(0, CMAP_AISLE_STACK);

  kernel.fw_.global_env_ = cmap_global_env_create();
}

/*******************************************************************************
*******************************************************************************/

static void delete_all()
{
  CMAP_WAREHOUSE * wh = kernel.fw_.warehouse_;

  cmap_delete_list_vals((CMAP_LIST *)CMAP_GET(wh, CMAP_AISLE_STACK));
  CMAP_CALL(kernel.fw_.pool_list_, delete);
  CMAP_CALL(kernel.fw_.pool_string_, delete);
  CMAP_CALL((CMAP_MAP *)wh, delete);
}

/*******************************************************************************
*******************************************************************************/

static void check_mem(int * ret)
{
  if(cmap_mem_public.is_this(kernel.mem_))
  {
    int s = cmap_mem_state() -> size_alloc;
    kernel.log_.debug("Allocated memory size : [%d].", s);
    if((s != 0) && kernel.cfg_ -> failure_on_allocmem) *ret = EXIT_FAILURE;
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
  state = CMAP_KERNEL_S_EXITING;

  if(!exiting)
  {
    exiting = CMAP_T;

    delete_all();

    check_all(&ret);

    kernel.log_.debug("Exit kernel (%d).", ret);
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
  return state;
}

/*******************************************************************************
*******************************************************************************/

static void cfg_init(CMAP_KERNEL_CFG * cfg)
{
  cfg -> mem = NULL;
  cfg -> failure_on_allocmem = CMAP_T;
}

/*******************************************************************************
*******************************************************************************/

inline CMAP_MEM * get_mem(CMAP_KERNEL_CFG * cfg)
{
  CMAP_MEM * mem = cfg -> mem;
  if(mem == NULL) mem = cmap_mem_public.create(0);
  return mem;
}

/*******************************************************************************
*******************************************************************************/

static void init(CMAP_KERNEL_CFG * cfg_)
{
  kernel_ptr = &kernel;
  state = CMAP_KERNEL_S_INIT;

  cmap_log_init(&kernel.log_);
  kernel.log_.debug("Init kernel.");

  if(cfg_ == NULL)
  {
    cfg_ = &cfg;
    cfg_init(cfg_);
  }
  kernel.cfg_ = cfg_;

  kernel.mem_ = get_mem(cfg_);

  kernel.main = kernel_main;
  kernel.exit = kernel_exit;
  kernel.fatal = kernel_fatal;
  kernel.state = kernel_state;

  init_fw_env();

  state = CMAP_KERNEL_S_ALIVE;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL * this()
{
  return kernel_ptr;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_KERNEL_PUBLIC cmap_kernel_public =
{
  init,
  this
};
