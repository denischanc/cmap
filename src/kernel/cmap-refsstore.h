#ifndef __CMAP_REFSSTORE_H__
#define __CMAP_REFSSTORE_H__

#include "cmap-lifecycle-type.h"
#include "cmap-map-type.h"

typedef struct CMAP_REFSSTORE CMAP_REFSSTORE;

struct CMAP_REFSSTORE
{
  void (*delete)(CMAP_REFSSTORE * this, CMAP_MAP * ret);

  void (*add)(CMAP_REFSSTORE * this, CMAP_LIFECYCLE * lc, char created);
};

typedef struct
{
  CMAP_REFSSTORE * (*create)();

  void (*log_consumed_time)(char lvl);
} CMAP_REFSSTORE_PUBLIC;

extern const CMAP_REFSSTORE_PUBLIC cmap_refsstore_public;

#endif
