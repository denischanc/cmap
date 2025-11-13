#ifndef __CMAP_FN_NAME_H__
#define __CMAP_FN_NAME_H__

typedef struct
{
  char * (*resolve)(const char * path);
  void (*resolve_to_config)(const char * path);
} CMAP_FN_NAME_PUBLIC;

extern const CMAP_FN_NAME_PUBLIC cmap_fn_name_public;

#endif
