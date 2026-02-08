#ifndef __CMAP_USAGE_H__
#define __CMAP_USAGE_H__

typedef struct
{
  void (*display_val_desc)(const char * val, const char * desc);

  int (*usage)(const char * desc, int * config_ids);
} CMAP_USAGE_PUBLIC;

extern const CMAP_USAGE_PUBLIC cmap_usage_public;

#endif
