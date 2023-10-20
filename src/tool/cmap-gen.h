#ifndef __CMAP_GEN_H__
#define __CMAP_GEN_H__

typedef struct
{
  int (*main)(int argc, char * argv[]);
  char (*relative_inc)();
} CMAP_GEN_PUBLIC;

extern const CMAP_GEN_PUBLIC cmap_gen_public;

#endif
