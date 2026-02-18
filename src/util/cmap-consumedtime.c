
#include "cmap-consumedtime.h"

#include <stdlib.h>
#include "cmap-log.h"

/*******************************************************************************
*******************************************************************************/

void cmap_consumedtime_start(CMAP_CONSUMEDTIME_US * ct)
{
  gettimeofday(&ct -> tv, NULL);
}

/*******************************************************************************
*******************************************************************************/

int64_t cmap_consumedtime_stop(CMAP_CONSUMEDTIME_US * ct)
{
  struct timeval cur_tv;
  gettimeofday(&cur_tv, NULL);

  int64_t diff_time = cur_tv.tv_sec - ct -> tv.tv_sec;
  diff_time *= 1000000;
  diff_time += cur_tv.tv_usec - ct -> tv.tv_usec;

  if(ct -> ok == 0) { ct -> ok = 1; ct -> min = ct -> max = diff_time; }
  else if(diff_time < ct -> min) ct -> min = diff_time;
  else if(diff_time > ct -> max) ct -> max = diff_time;
  ct -> nb++;
  ct -> total += diff_time;

  return diff_time;
}

/*******************************************************************************
*******************************************************************************/

void cmap_consumedtime_log(char lvl, CMAP_CONSUMEDTIME_US * ct,
  const char * what)
{
  cmap_log(lvl, "Consumed time (us) in %s : %ld x %ld/%ld ~= %ld.",
    what, ct -> nb, ct -> min, ct -> max, ct -> total);
}
