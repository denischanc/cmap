#ifndef __CMAP_LIFECYCLE_INT_H__
#define __CMAP_LIFECYCLE_INT_H__

#include <stdint.h>

typedef struct
{
  unsigned char nature, ref_state;

  int nb_refs;

  uint64_t watch_time_us;
} CMAP_LIFECYCLE_INTERNAL;

#endif
