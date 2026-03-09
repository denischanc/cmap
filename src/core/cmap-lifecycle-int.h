#ifndef __CMAP_LIFECYCLE_INT_H__
#define __CMAP_LIFECYCLE_INT_H__

#include <stdint.h>
#include "cmap-env-type.h"

typedef struct
{
  const char * nature;

  int nb_refs;

  uint64_t watch_time_us;
  const char * ref_state;
} CMAP_LIFECYCLE_INTERNAL;

#endif
