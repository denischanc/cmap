#ifndef __CMAP_LIFECYCLE_DEFINE_H__
#define __CMAP_LIFECYCLE_DEFINE_H__

#include "cmap.h"

#define CMAP_INC_REF(lc) CMAP_CALL((CMAP_LIFECYCLE *)lc, inc_ref)
#define CMAP_DEC_REF(lc) CMAP_CALL((CMAP_LIFECYCLE *)lc, dec_ref)

#endif
