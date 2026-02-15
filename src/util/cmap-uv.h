#ifndef __CMAP_UV_H__
#define __CMAP_UV_H__

#include <uv.h>

void cmap_uv_loop_run();

void cmap_uv_idle_start(uv_idle_t * idle, uv_idle_cb cb);
void cmap_uv_idle_stop(uv_idle_t * idle, char free);

void cmap_uv_timer_start(uv_timer_t * timer, uv_timer_cb cb, uint64_t timeout,
  uint64_t repeat);
void cmap_uv_timer_stop(uv_timer_t * timer, char free);

uint64_t cmap_uv_now_ms();

#endif
