#ifndef ___CMAP_FN_H___
#define ___CMAP_FN_H___

#include <cmap/core.h>

typedef CMAP_MAP * (*CMAP_FN_TPL)(CMAP_MAP * features, CMAP_MAP * map,
  CMAP_LIST * args);

extern const char * CMAP_PROTOTYPE_NAME;

#endif
