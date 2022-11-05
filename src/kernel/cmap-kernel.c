
#include "cmap-kernel.h"

#include <stdlib.h>
#include "cmap-global-env.h"
#include "cmap-aisle.h"
#include "cmap-util.h"
#include "cmap-mem.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_KERNEL_CFG * cfg;

  CMAP_MEM * mem;
  CMAP_LOG * log;

  CMAP_AISLESTORE * aislestore;

  CMAP_POOL_LIST * pool_list;
  CMAP_POOL_STRING * pool_string;

  CMAP_MAP * global_env;

  char exiting;

  int state;
} INTERNAL;

static INTERNAL internal = {NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  CMAP_F, CMAP_KERNEL_S_UNKNOWN};

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL kernel = {};
static CMAP_KERNEL * kernel_ptr = NULL;

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL_CFG * instance_cfg()
{
  static CMAP_KERNEL_CFG cfg = {};

  cfg.failure_on_allocmem = CMAP_T;
  cfg.mem = NULL;
  cfg.log = NULL;

  return &cfg;
}

static CMAP_KERNEL_CFG * cfg()
{
  if(internal.cfg == NULL) internal.cfg = instance_cfg();
  return internal.cfg;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MEM * mem()
{
  if(internal.mem == NULL)
  {
    internal.mem = cfg() -> mem;
    if(internal.mem == NULL) internal.mem = cmap_mem_public.init(0);
  }
  return internal.mem;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_LOG * log_()
{
  if(internal.log == NULL)
  {
    internal.log = cfg() -> log;
    if(internal.log == NULL) internal.log = cmap_log_public.init();
  }
  return internal.log;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_AISLESTORE * aislestore()
{
  if(internal.aislestore == NULL)
  {
    internal.aislestore = cmap_aislestore_public.create();
  }
  return internal.aislestore;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_POOL_LIST * pool_list()
{
  if(internal.pool_list == NULL)
  {
    internal.pool_list = cmap_pool_list_public.create(20);
  }
  return internal.pool_list;
}

static CMAP_POOL_STRING * pool_string()
{
  if(internal.pool_string == NULL)
  {
    internal.pool_string = cmap_pool_string_public.create(20);
  }
  return internal.pool_string;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MAP * global_env()
{
  if(internal.global_env == NULL)
  {
    internal.global_env = cmap_global_env_public.create();
  }
  return internal.global_env;
}

/*******************************************************************************
*******************************************************************************/

static void delete_all()
{
  CMAP_CALL(pool_list(), delete);
  CMAP_CALL(pool_string(), delete);

  CMAP_MAP * as = (CMAP_MAP *)aislestore();
  CMAP_CALL(as, delete);
}

/*******************************************************************************
*******************************************************************************/

static void check_mem(int * ret)
{
  if(cmap_mem_public.is_this(internal.mem))
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

static CMAP_KERNEL * init(CMAP_KERNEL_CFG * cfg)
{
  if(kernel_ptr == NULL)
  {
    internal.state = CMAP_KERNEL_S_INIT;
    internal.cfg = cfg;

    kernel.main = main_;
    kernel.exit = exit_;
    kernel.fatal = fatal;
    kernel.state = state;

    kernel_ptr = &kernel;

    cmap_log_public.debug("Init kernel.");

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
  instance,
  mem,
  log_,
  aislestore,
  pool_list,
  pool_string,
  global_env
};
