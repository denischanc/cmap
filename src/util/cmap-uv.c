
#include "cmap-uv.h"

#include "cmap.h"
#include "cmap-log.h"
#include "cmap-mem.h"

/*******************************************************************************
*******************************************************************************/

static uv_loop_t loop_static, * loop_ptr = NULL;

static char loop_closed = CMAP_F;

/*******************************************************************************
*******************************************************************************/

static void error(int err)
{
  if(err < 0) cmap_log_public.fatal("Libuv : %s", uv_strerror(err));
}

/*******************************************************************************
*******************************************************************************/

static uv_loop_t * loop()
{
  if(loop_ptr == NULL)
  {
    loop_ptr = &loop_static;
    error(uv_loop_init(loop_ptr));
  }
  return loop_ptr;
}

/*******************************************************************************
*******************************************************************************/

static void loop_run()
{
  uv_loop_t * loop_ = loop();
  error(uv_run(loop_, UV_RUN_DEFAULT));
  error(uv_loop_close(loop_));
  loop_closed = CMAP_T;
}

/*******************************************************************************
*******************************************************************************/

static void free_handle(uv_handle_t * handle)
{
  CMAP_MEM_INSTANCE_FREE(handle);
}

/*******************************************************************************
*******************************************************************************/

static void idle_start(uv_idle_t * idle, uv_idle_cb cb)
{
  if(loop_closed) return;
  error(uv_idle_init(loop(), idle));
  error(uv_idle_start(idle, cb));
}

static void idle_stop(uv_idle_t * idle, char free)
{
  if(loop_closed) return;
  error(uv_idle_stop(idle));
  uv_close((uv_handle_t *)idle, free ? free_handle : NULL);
}

/*******************************************************************************
*******************************************************************************/

static void timer_start(uv_timer_t * timer, uv_timer_cb cb, uint64_t timeout,
  uint64_t repeat)
{
  if(loop_closed) return;
  error(uv_timer_init(loop(), timer));
  error(uv_timer_start(timer, cb, timeout, repeat));
}

static void timer_stop(uv_timer_t * timer, char free)
{
  if(loop_closed) return;
  error(uv_timer_stop(timer));
  uv_close((uv_handle_t *)timer, free ? free_handle : NULL);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_UV_PUBLIC cmap_uv_public =
{
  loop_run,
  idle_start, idle_stop,
  timer_start, timer_stop
};
