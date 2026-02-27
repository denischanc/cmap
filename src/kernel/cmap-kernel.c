
#include "cmap-kernel.h"

#include <stdlib.h>
#include "cmap-log.h"
#include "cmap-mem.h"
#include "cmap-env.h"
#include "cmap-loop.h"
#include "cmap-config.h"
#include "cmap-kcli.h"

#ifdef CONSUMED_TIME
#include "cmap-refsstore.h"
#include "cmap-refswatcher.h"
#endif

/*******************************************************************************
*******************************************************************************/

static char state = CMAP_KERNEL_S_ALIVE;

/*******************************************************************************
*******************************************************************************/

static void check_mem(int * ret)
{
  if(cmap_mem_is_internal())
  {
    int s = cmap_mem_state() -> size_alloc;
    cmap_log_info("Allocated memory size : [%d].", s);
    if((s != 0) && cmap_config_mem_failure_on_alloc()) *ret = EXIT_FAILURE;

#ifdef CONSUMED_TIME
    cmap_mem_log_consumed_time(CMAP_LOG_INFO);
#endif
  }
}

static void check_refs()
{
#ifdef CONSUMED_TIME
  cmap_refsstore_log_consumed_time(CMAP_LOG_INFO);
  cmap_refswatcher_log_consumed_time(CMAP_LOG_INFO);
#endif
}

static void check_all(int * ret)
{
  check_mem(ret);
  check_refs();
}

/*******************************************************************************
*******************************************************************************/

void cmap_kernel_exit(int ret)
{
  if(state != CMAP_KERNEL_S_EXITING)
  {
    state = CMAP_KERNEL_S_EXITING;

    cmap_env_delete_all();

    check_all(&ret);
    cmap_log_info("Exit kernel (%d).", ret);

    cmap_log_delete();
    cmap_mem_delete();

    exit(ret);
  }
}

void cmap_kernel_fatal()
{
  cmap_kernel_exit(EXIT_FAILURE);
}

/*******************************************************************************
*******************************************************************************/

int cmap_kernel_main()
{
  cmap_log_info("Kernel start loop ...");
  cmap_loop_run();
  cmap_log_info("Loop terminated.");

  cmap_kernel_exit(EXIT_SUCCESS);
  return EXIT_SUCCESS;
}

/*******************************************************************************
*******************************************************************************/

char cmap_kernel_state()
{
  return state;
}

/*******************************************************************************
*******************************************************************************/

void cmap_kernel_bootstrap(int argc, char ** argv)
{
  cmap_kcli_parse(argc, argv);
}
