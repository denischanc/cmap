#ifndef __CMAP_PROTOTYPESTORE_H__
#define __CMAP_PROTOTYPESTORE_H__

#include "cmap-prototypestore-define.h"
#include "cmap-lifecycle.h"

typedef struct CMAP_PROTOTYPESTORE CMAP_PROTOTYPESTORE;

struct CMAP_PROTOTYPESTORE
{
  CMAP_LIFECYCLE super;

  void * internal;

  CMAP_PROTOTYPESTORE_LOOP(CMAP_PROTOTYPESTORE_DECL)
};

typedef struct
{
  CMAP_PROTOTYPESTORE * (*create)(CMAP_PROC_CTX * proc_ctx);
} CMAP_PROTOTYPESTORE_PUBLIC;

extern const CMAP_PROTOTYPESTORE_PUBLIC cmap_prototypestore_public;

#endif
