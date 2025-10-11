
#include "cmap-uv.h"

#include "cmap-log.h"

/*******************************************************************************
*******************************************************************************/

static uv_loop_t loop_static, * loop_ptr = NULL;

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
  error(uv_run(loop(), UV_RUN_DEFAULT));
}

static void loop_close()
{
  if(!uv_loop_alive(loop())) error(uv_loop_close(loop()));
}

/*******************************************************************************
*******************************************************************************/

static void idle_start(uv_idle_t * idle, uv_idle_cb cb)
{
  error(uv_idle_init(loop(), idle));
  error(uv_idle_start(idle, cb));
}

static void idle_stop(uv_idle_t * idle)
{
  error(uv_idle_stop(idle));
  uv_close((uv_handle_t *)idle, NULL);
}

/*******************************************************************************
*******************************************************************************/

static void timer_start(uv_timer_t * timer, uv_timer_cb cb, uint64_t timeout,
  uint64_t repeat)
{
  error(uv_timer_init(loop(), timer));
  error(uv_timer_start(timer, cb, timeout, repeat));
}

static void timer_stop(uv_timer_t * timer)
{
  error(uv_timer_stop(timer));
  uv_close((uv_handle_t *)timer, NULL);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_UV_PUBLIC cmap_uv_public =
{
  loop_run, loop_close,
  idle_start, idle_stop,
  timer_start, timer_stop
};
