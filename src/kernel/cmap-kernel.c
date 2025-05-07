
#include "cmap-kernel.h"

#include <stdlib.h>
#include "cmap.h"
#include "cmap-util.h"
#include "cmap-log.h"
#include "cmap-env.h"
#include "cmap-refsstore.h"

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  CMAP_KERNEL_CFG * cfg;

  CMAP_MEM * mem;
  CMAP_LOG * log;

  uv_loop_t * uv_loop;

  char state;
} INTERNAL;

static INTERNAL internal = {NULL, NULL, NULL, NULL, CMAP_KERNEL_S_UNKNOWN};

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL kernel = {};
static CMAP_KERNEL * kernel_ptr = NULL;

static CMAP_KERNEL_CFG kernel_cfg = {};
static CMAP_KERNEL_CFG * kernel_cfg_ptr = NULL;

/*******************************************************************************
*******************************************************************************/

static CMAP_KERNEL_CFG * dft_cfg()
{
  if(kernel_cfg_ptr == NULL)
  {
    kernel_cfg.mem.this = NULL;
    kernel_cfg.mem.chunk_size = 1 << 20;
    kernel_cfg.mem.failure_on_alloc = CMAP_T;
    kernel_cfg.log.this = NULL;
    kernel_cfg.log.lvl = CMAP_LOG_FATAL;
    kernel_cfg.log.path = NULL;
    kernel_cfg.refs.check_zombie_time_us = 3000000;
    kernel_cfg.core.list_chunk_size = 1 << 8;
    kernel_cfg.core.string_size_inc_min = 1 << 6;
    kernel_cfg.core.string_size_inc = 1 << 10;
    kernel_cfg.pool.size = 1 << 10;

    kernel_cfg_ptr = &kernel_cfg;
  }
  return kernel_cfg_ptr;
}

static CMAP_KERNEL_CFG * cfg_()
{
  if(internal.cfg == NULL) internal.cfg = dft_cfg();
  return internal.cfg;
}

/*******************************************************************************
*******************************************************************************/

static CMAP_MEM * mem()
{
  if(internal.mem == NULL)
  {
    internal.mem = cfg_() -> mem.this;
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
    internal.log = cfg_() -> log.this;
    if(internal.log == NULL) internal.log = cmap_log_public.instance();
  }
  return internal.log;
}

/*******************************************************************************
*******************************************************************************/

static uv_loop_t * this_uv_loop()
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
    cmap_log_public.info("Allocated memory size : [%d].", s);
    if((s != 0) && internal.cfg -> mem.failure_on_alloc) *ret = EXIT_FAILURE;

#ifdef CONSUMED_TIME
    cmap_mem_public.log_consumed_time(CMAP_LOG_INFO);
#endif
  }
}

static void check_refs()
{
#ifdef CONSUMED_TIME
  cmap_refsstore_public.log_consumed_time(CMAP_LOG_INFO);
  cmap_refswatcher_public.log_consumed_time(CMAP_LOG_INFO);
#endif
}

static void check_all(int * ret)
{
  check_mem(ret);
  check_refs();
}

/*******************************************************************************
*******************************************************************************/

static void delete_all()
{
  cmap_env_public.delete_all();

  uv_loop_close(this_uv_loop());
  CMAP_MEM_FREE(this_uv_loop(), mem());
}

static void exit_(int ret)
{
  if(internal.state != CMAP_KERNEL_S_EXITING)
  {
    internal.state = CMAP_KERNEL_S_EXITING;

    delete_all();

    check_all(&ret);
    cmap_log_public.info("Exit kernel (%d).", ret);

    if(internal.log != NULL) internal.log -> delete();
    if(internal.mem != NULL) internal.mem -> delete();

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
  cmap_log_public.info("Kernel start uv loop ...");
  cmap_util_public.uv_error(uv_run(this_uv_loop(), UV_RUN_DEFAULT));
  cmap_log_public.info("Uv loop terminated.");

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
    kernel.cfg = cfg_;
    kernel.mem = mem;
    kernel.log = log_;
    kernel.uv_loop = this_uv_loop;
    kernel.state = state;

    kernel_ptr = &kernel;

    internal.state = CMAP_KERNEL_S_ALIVE;
    cmap_log_public.info("Alive kernel.");
  }
  return kernel_ptr;
}

static CMAP_KERNEL * instance()
{
  return bootstrap(NULL);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_KERNEL_PUBLIC cmap_kernel_public =
{
  dft_cfg,
  bootstrap,
  instance
};
