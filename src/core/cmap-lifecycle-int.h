#ifndef __CMAP_LIFECYCLE_INT_H__
#define __CMAP_LIFECYCLE_INT_H__

#include <stdint.h>

typedef struct
{
  unsigned char nature, ref_state;
  char watched, ghost;

  int nb_refs;
} CMAP_LIFECYCLE_INTERNAL;

#endif
