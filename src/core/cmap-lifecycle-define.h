#ifndef __CMAP_LIFECYCLE_DEFINE_H__
#define __CMAP_LIFECYCLE_DEFINE_H__

#include "cmap.h"

#define CMAP_DELETE(lc) CMAP_CALL((CMAP_LIFECYCLE *)lc, delete)

#endif
