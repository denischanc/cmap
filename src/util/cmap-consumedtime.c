
#include "cmap-consumedtime.h"

#include <stdlib.h>

/*******************************************************************************
*******************************************************************************/

static void start(struct timeval * local_time)
{
  gettimeofday(local_time, NULL);
}

/*******************************************************************************
*******************************************************************************/

static int64_t stop_us(struct timeval * local_time, int64_t * global_time_us)
{
  struct timeval cur_local_time;
  gettimeofday(&cur_local_time, NULL);

  int diff_time_us = cur_local_time.tv_sec - local_time -> tv_sec;
  diff_time_us *= 1000000;
  diff_time_us += cur_local_time.tv_usec - local_time -> tv_usec;
  if(global_time_us != NULL) (*global_time_us) += diff_time_us;
  return diff_time_us;
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CONSUMEDTIME_PUBLIC cmap_consumedtime_public = { start, stop_us };
