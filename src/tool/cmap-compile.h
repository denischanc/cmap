#ifndef __CMAP_COMPILE_H__
#define __CMAP_COMPILE_H__

#define CMAP_COMPILE_MODULE_NAME "compile"

typedef struct
{
  int (*main)(int argc, char * argv[]);
} CMAP_COMPILE_PUBLIC;

extern const CMAP_COMPILE_PUBLIC cmap_compile_public;

#endif
