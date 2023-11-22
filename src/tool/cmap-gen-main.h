#ifndef __CMAP_GEN_MAIN_H__
#define __CMAP_GEN_MAIN_H__

typedef struct
{
  void (*impl)(char ** txt);

  int (*main)(int argc, char * argv[]);
} CMAP_GEN_MAIN_PUBLIC;

extern const CMAP_GEN_MAIN_PUBLIC cmap_gen_main_public;

#endif
