#ifndef __CMAP_CONSUMEDTIME_H__
#define __CMAP_CONSUMEDTIME_H__

#include <sys/time.h>
#include <stdint.h>

typedef struct
{
  void (*start)(struct timeval * local_time);
  int64_t (*stop_us)(struct timeval * local_time, int64_t * global_time_us);
} CMAP_CONSUMEDTIME_PUBLIC;

extern const CMAP_CONSUMEDTIME_PUBLIC cmap_consumedtime_public;

#endif
