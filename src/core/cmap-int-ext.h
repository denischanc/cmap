#ifndef __CMAP_INT_EXT_H__
#define __CMAP_INT_EXT_H__

#include "cmap-int-type.h"
#include <stdint.h>

int64_t cmap_int_get(CMAP_INT * i);
CMAP_INT * cmap_int_set(CMAP_INT * i, int64_t val);

extern const char * CMAP_INT_NATURE;

#endif
