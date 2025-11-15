#ifndef __CMAP_BUILD_H__
#define __CMAP_BUILD_H__

#define CMAP_BUILD_MODULE_NAME "build"

typedef struct
{
  int (*main)(int argc, char * argv[]);
} CMAP_BUILD_PUBLIC;

extern const CMAP_BUILD_PUBLIC cmap_build_public;

#endif
