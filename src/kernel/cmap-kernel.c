
#include "cmap-kernel.h"

#include <stdlib.h>
#include "cmap-log.h"
#include "cmap-mem.h"
#include "cmap-env.h"
#include "cmap-uv.h"
#include "cmap-config.h"
#include "cmap-kcli.h"

#ifdef CONSUMED_TIME
#include "cmap-refsstore.h"
#include "cmap-refswatcher.h"
#endif

/*******************************************************************************
*******************************************************************************/

typedef struct
{
  char state;
} INTERNAL;

static INTERNAL internal = {CMAP_KERNEL_S_ALIVE};

/*******************************************************************************
*******************************************************************************/

static void check_mem(int * ret)
{
  if(cmap_mem_public.is_this())
  {
    int s = cmap_mem_public.state() -> size_alloc;
    cmap_log_public.info("Allocated memory size : [%d].", s);
    if((s != 0) && cmap_config_mem_failure_on_alloc()) *ret = EXIT_FAILURE;

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

  cmap_uv_public.loop_close();
}

static void exit_(int ret)
{
  if(internal.state != CMAP_KERNEL_S_EXITING)
  {
    internal.state = CMAP_KERNEL_S_EXITING;

    delete_all();

    check_all(&ret);
    cmap_log_public.info("Exit kernel (%d).", ret);

    cmap_log_public.instance() -> delete();
    cmap_mem_public.instance() -> delete();

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
  cmap_uv_public.loop_run();
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

static CMAP_KERNEL kernel = {main_, exit_, fatal, state};

static CMAP_KERNEL * instance()
{
  return &kernel;
}

/*******************************************************************************
*******************************************************************************/

static void bootstrap(int argc, char ** argv)
{
  cmap_kcli_public.parse(argc, argv);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_KERNEL_PUBLIC cmap_kernel_public = {instance, bootstrap};
