#ifndef __CMAP_UV_H__
#define __CMAP_UV_H__

#include <uv.h>

typedef struct
{
  void (*loop_run)();
  void (*loop_close)();

  void (*idle_start)(uv_idle_t * idle, uv_idle_cb cb);
  void (*idle_stop)(uv_idle_t * idle);

  void (*timer_start)(uv_timer_t * timer, uv_timer_cb cb, uint64_t timeout,
    uint64_t repeat);
  void (*timer_stop)(uv_timer_t * timer);
} CMAP_UV_PUBLIC;

extern const CMAP_UV_PUBLIC cmap_uv_public;

#endif
