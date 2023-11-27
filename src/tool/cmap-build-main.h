#ifndef __CMAP_BUILD_MAIN_H__
#define __CMAP_BUILD_MAIN_H__

#define CMAP_BUILD_MAIN_MODULE_NAME "build-main"

typedef struct
{
  void (*impl)(char ** txt);

  int (*main)(int argc, char * argv[]);
} CMAP_BUILD_MAIN_PUBLIC;

extern const CMAP_BUILD_MAIN_PUBLIC cmap_build_main_public;

#endif
