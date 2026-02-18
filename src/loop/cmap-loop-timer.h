#ifndef __CMAP_LOOP_TIMER_H__
#define __CMAP_LOOP_TIMER_H__

#include "cmap-loop-timer-type.h"
#include <stdint.h>
#include "cmap-loop-timer-int.h"
#include "cmap-loop.h"

struct CMAP_LOOP_TIMER
{
  CMAP_LOOP_EVENT super;

  CMAP_LOOP_TIMER_INTERNAL internal;

  void * data;
};

void cmap_loop_timer_start(CMAP_LOOP_TIMER * timer, CMAP_LOOP_TIMER_CB cb,
  uint64_t timeout_ms, uint64_t repeat_ms);
void cmap_loop_timer_stop(CMAP_LOOP_TIMER * timer);

char cmap_loop_timer_is_stopped(CMAP_LOOP_TIMER * timer);

#endif
