
#include "cmap-kernel.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-util.h"
#include "cmap-log.h"
#include "cmap-env.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_KERNEL_CFG * cfg;

  CMAP_MEM * mem;
  CMAP_LOG * log;

  uv_loop_t * uv_loop;

  char exiting, state;
} INTERNAL;

static INTERNAL internal = {NULL, NULL, NULL, NULL, CMAP_F,
  CMAP_KERNEL_S_UNKNOWN};

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
    if(internal.mem == NULL) internal.mem = cmap_mem_public.instance(0);
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
    if(internal.log == NULL) internal.log = cmap_log_public.instance();
  }
  return internal.log;
}

/*******************************************************************************
*******************************************************************************/

static uv_loop_t * uv_loop()
{
  if(internal.uv_loop == NULL)
  {
    internal.uv_loop = CMAP_MEM_ALLOC(uv_loop_t, mem());
    cmap_util_public.uv_error(uv_loop_init(internal.uv_loop));
  }
  return internal.uv_loop;
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

static void delete_all()
{
  cmap_env_public.delete_all();

  CMAP_MEM_FREE(uv_loop(), mem());
}

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

static int main_()
{
  cmap_util_public.uv_error(uv_run(uv_loop(), UV_RUN_DEFAULT));

  exit_(EXIT_SUCCESS);
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

static char state()
{
  return internal.state;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL * bootstrap(CMAP_KERNEL_CFG * cfg)
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
  bootstrap,
  instance,
  mem,
  log_,
  uv_loop
};
