#ifndef __CMAP_MAIN_USAGE_H__
#define __CMAP_MAIN_USAGE_H__

typedef struct
{
  void (*usage)(char * argv[]);
  void (*usage_gen)(char * argv[]);
  void (*usage_prj)(char * argv[]);
} CMAP_MAIN_USAGE_PUBLIC;

extern const CMAP_MAIN_USAGE_PUBLIC cmap_main_usage_public;

#endif
