#ifndef __CMAP_CONFIG_H__
#define __CMAP_CONFIG_H__

#include "cmap-config-type.h"

typedef struct
{
  CMAP_CONFIG * (*instance)();

  void (*init)(int argc, char ** argv);

  int (*argc)();
  char ** (*argv)();
} CMAP_CONFIG_PUBLIC;

extern const CMAP_CONFIG_PUBLIC cmap_config_public;

#endif
