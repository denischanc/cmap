#ifndef __CMAP_CONSUMEDTIME_H__
#define __CMAP_CONSUMEDTIME_H__

#include <sys/time.h>
#include <stdint.h>

typedef struct
{
  struct timeval tv;

  int64_t min, max, nb, total;

  char ok;
} CMAP_CONSUMEDTIME_US;

void cmap_consumedtime_start(CMAP_CONSUMEDTIME_US * ct);
int64_t cmap_consumedtime_stop(CMAP_CONSUMEDTIME_US * ct);

void cmap_consumedtime_log(char lvl, CMAP_CONSUMEDTIME_US * ct,
  const char * what);

#endif
