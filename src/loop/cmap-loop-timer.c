
#include "cmap-loop-timer.h"

#include <stddef.h>
#include "cmap-util.h"

/*******************************************************************************
*******************************************************************************/

static char fired(CMAP_LOOP_EVENT * event, uint64_t time_us)
{
  return (((CMAP_LOOP_TIMER *)event) -> internal.next_time_us <= time_us);
}

/*******************************************************************************
*******************************************************************************/

static void run(CMAP_LOOP_EVENT * event, uint64_t time_us)
{
  CMAP_LOOP_TIMER * timer = (CMAP_LOOP_TIMER *)event;

  uint64_t repeat_us = timer -> internal.repeat_us;
  if(repeat_us == 0) cmap_loop_timer_stop(timer);
  else
  {
    uint64_t * next_time_us = &timer -> internal.next_time_us;
    while(*next_time_us <= time_us) *next_time_us += repeat_us;
  }

  timer -> internal.run(timer);
}

/*******************************************************************************
*******************************************************************************/

static void delete(CMAP_LOOP_EVENT * event, uint64_t time_us)
{
  cmap_loop_rm(event);

  CMAP_LOOP_TIMER * timer = (CMAP_LOOP_TIMER *)event;
  timer -> internal.delete(timer);
}

/*******************************************************************************
*******************************************************************************/

void cmap_loop_timer_start(CMAP_LOOP_TIMER * timer, CMAP_LOOP_TIMER_CB run_,
  uint64_t timeout_ms, uint64_t repeat_ms, CMAP_LOOP_TIMER_CB delete_)
{
  timer -> internal.run = run_;
  timer -> internal.delete = delete_;
  timer -> internal.next_time_us = cmap_util_time_us() + timeout_ms * 1000;
  timer -> internal.repeat_us = repeat_ms * 1000;

  CMAP_LOOP_EVENT * event = &timer -> super;
  event -> fired = fired;
  event -> run = run;
  cmap_loop_add(event);
}

void cmap_loop_timer_stop(CMAP_LOOP_TIMER * timer)
{
  if(timer -> internal.delete == NULL) cmap_loop_rm((CMAP_LOOP_EVENT *)timer);
  else
  {
    timer -> super.fired = cmap_loop_fired_true;
    timer -> super.run = delete;
  }
}
