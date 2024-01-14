#ifndef __CMAP_CONSUMEDTIME_H__
#define __CMAP_CONSUMEDTIME_H__

#include <sys/time.h>
#include <stdint.h>

typedef struct
{
  struct timeval tv;

  int64_t min, max, nb, total;
} CMAP_CONSUMEDTIME_US;

typedef struct
{
  void (*start)(CMAP_CONSUMEDTIME_US * ct);
  int64_t (*stop)(CMAP_CONSUMEDTIME_US * ct);

  void (*log)(char lvl, CMAP_CONSUMEDTIME_US * ct, const char * what);
} CMAP_CONSUMEDTIME_PUBLIC;

extern const CMAP_CONSUMEDTIME_PUBLIC cmap_consumedtime_public;

#endif
