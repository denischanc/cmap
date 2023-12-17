#ifndef __CMAP_LIFECYCLE_DEFINE_H__
#define __CMAP_LIFECYCLE_DEFINE_H__

#include "cmap.h"

#define CMAP_NATURE(lc) CMAP_CALL((CMAP_LIFECYCLE *)lc, nature)

#define CMAP_INC_REFS(lc) CMAP_CALL((CMAP_LIFECYCLE *)lc, inc_refs)
#define CMAP_DEC_REFS(lc) CMAP_CALL((CMAP_LIFECYCLE *)lc, dec_refs)

#endif
