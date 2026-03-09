#ifndef __CMAP_PROTOTYPESTORE_H__
#define __CMAP_PROTOTYPESTORE_H__

#include "cmap-prototypestore-define.h"

typedef struct CMAP_PROTOTYPESTORE CMAP_PROTOTYPESTORE;

CMAP_PROTOTYPESTORE_LOOP(CMAP_PROTOTYPESTORE_DECL)

void cmap_prototypestore_delete(CMAP_PROTOTYPESTORE * ps,
  CMAP_PROC_CTX * proc_ctx);
CMAP_PROTOTYPESTORE * cmap_prototypestore_create(CMAP_PROC_CTX * proc_ctx);

#endif
