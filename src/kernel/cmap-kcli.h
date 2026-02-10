#ifndef __CMAP_KCLI_H__
#define __CMAP_KCLI_H__

#include "cmap-kcli-define.h"

typedef struct
{
  CMAP_KCLI_LOOP(CMAP_KCLI_DECL)

  void (*parse)(int argc, char ** argv);
  int (*argc)();
  char ** (*argv)();
} CMAP_KCLI_PUBLIC;

extern const CMAP_KCLI_PUBLIC cmap_kcli_public;

#endif
