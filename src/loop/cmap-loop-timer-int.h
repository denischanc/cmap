#ifndef __CMAP_LOOP_TIMER_INT_H__
#define __CMAP_LOOP_TIMER_INT_H__

typedef struct
{
  CMAP_LOOP_TIMER_CB cb;

  uint64_t next_time_us, repeat_us;
} CMAP_LOOP_TIMER_INTERNAL;

#endif
