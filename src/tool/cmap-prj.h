#ifndef __CMAP_PRJ_H__
#define __CMAP_PRJ_H__

#define CMAP_PRJ_MODULE_NAME "prj"

typedef struct
{
  int (*main)(int argc, char * argv[]);
} CMAP_PRJ_PUBLIC;

extern const CMAP_PRJ_PUBLIC cmap_prj_public;

#endif
