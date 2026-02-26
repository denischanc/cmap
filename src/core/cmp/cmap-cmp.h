#ifndef __CMAP_CMP_H__
#define __CMAP_CMP_H__

#include <stdint.h>
#include "cmap-map-type.h"

int64_t cmap_cmp_run(CMAP_MAP * map_l, CMAP_MAP * map_r);

char cmap_cmp_is_true(CMAP_MAP * map);

#endif
