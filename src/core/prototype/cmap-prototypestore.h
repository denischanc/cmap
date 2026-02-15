#ifndef __CMAP_PROTOTYPESTORE_H__
#define __CMAP_PROTOTYPESTORE_H__

#include "cmap-prototypestore-define.h"
#include "cmap-lifecycle.h"

typedef struct CMAP_PROTOTYPESTORE CMAP_PROTOTYPESTORE;

struct CMAP_PROTOTYPESTORE
{
  CMAP_LIFECYCLE super;

  CMAP_PROTOTYPESTORE_LOOP(CMAP_PROTOTYPESTORE_DECL)
};

CMAP_PROTOTYPESTORE * cmap_prototypestore_create(CMAP_PROC_CTX * proc_ctx);

#endif
