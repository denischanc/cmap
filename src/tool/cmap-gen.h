#ifndef __CMAP_GEN_H__
#define __CMAP_GEN_H__

typedef struct
{
  char * (*fn_name)(const char * base_name);

  int (*main)(int argc, char * argv[]);
} CMAP_GEN_PUBLIC;

extern const CMAP_GEN_PUBLIC cmap_gen_public;

#endif
