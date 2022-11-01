
#include "cmap-kernel.h"

#include <stdlib.h>
#include "cmap-global-env.h"
#include "cmap-aisle.h"
#include "cmap-util.h"
#include "cmap-prototype.h"
#include "cmap-mem.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_KERNEL_CFG * cfg;

  char exiting;

  int state;
} INTERNAL;

static INTERNAL internal = {NULL, CMAP_F, CMAP_KERNEL_S_UNKNOWN};

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL kernel = {};
static CMAP_KERNEL * kernel_ptr = NULL;

/*******************************************************************************
*******************************************************************************/

static void init_env()
{
  kernel.aislestore = cmap_aislestore_public.create();

  cmap_prototype_public.init();
  kernel.pool_list = cmap_pool_list_public.create(20);
  kernel.pool_string = cmap_pool_string_public.create(20);

  CMAP_LIST(0, CMAP_AISLE_STACK);

  kernel.global_env = cmap_global_env_public.create();
}

/*******************************************************************************
*******************************************************************************/

static void delete_all()
{
  CMAP_AISLESTORE * as = kernel.aislestore;

  cmap_util_public.delete_list_vals(
    (CMAP_LIST *)CMAP_GET(as, CMAP_AISLE_STACK));
  CMAP_CALL(kernel.pool_list, delete);
  CMAP_CALL(kernel.pool_string, delete);
  CMAP_CALL((CMAP_MAP *)as, delete);
}

/*******************************************************************************
*******************************************************************************/

static void check_mem(int * ret)
{
  if(cmap_mem_public.is_this(kernel.mem))
  {
    int s = cmap_mem_public.state() -> size_alloc;
    cmap_log_public.debug("Allocated memory size : [%d].", s);
    if((s != 0) && internal.cfg -> failure_on_allocmem) *ret = EXIT_FAILURE;
  }
}

static void check_all(int * ret)
{
  check_mem(ret);
}

/*******************************************************************************
*******************************************************************************/

static void exit_(int ret)
{
  internal.state = CMAP_KERNEL_S_EXITING;

  if(!internal.exiting)
  {
    internal.exiting = CMAP_T;

    delete_all();

    check_all(&ret);

    cmap_log_public.debug("Exit kernel (%d).", ret);
    exit(ret);
  }
}

static void fatal()
{
  exit_(EXIT_FAILURE);
}

/*******************************************************************************
*******************************************************************************/

static int main_(int argc, char * argv[])
{
  exit_(EXIT_SUCCESS);
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

static int state()
{
  return internal.state;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL_CFG * instance_cfg()
{
  static CMAP_KERNEL_CFG cfg = {};

  cfg.mem = NULL;
  cfg.failure_on_allocmem = CMAP_T;

  return &cfg;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MEM * get_mem()
{
  CMAP_MEM * mem = internal.cfg -> mem;
  if(mem == NULL) mem = cmap_mem_public.init(0);
  return mem;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL * init(CMAP_KERNEL_CFG * cfg)
{
  if(kernel_ptr == NULL)
  {
    internal.state = CMAP_KERNEL_S_INIT;

    if(cfg == NULL) cfg = instance_cfg();
    internal.cfg = cfg;

    kernel.mem = get_mem();

    kernel.main = main_;
    kernel.exit = exit_;
    kernel.fatal = fatal;
    kernel.state = state;

    kernel_ptr = &kernel;

    kernel.log = cmap_log_public.init();
    cmap_log_public.debug("Init kernel.");

    init_env();

    internal.state = CMAP_KERNEL_S_ALIVE;
  }
  return kernel_ptr;
}

static CMAP_KERNEL * instance()
{
  return kernel_ptr;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_KERNEL_PUBLIC cmap_kernel_public =
{
  init,
  instance
};
