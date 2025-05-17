#ifndef __CMAP_UTIL_H__
#define __CMAP_UTIL_H__

typedef struct
{
  char (*static_contains)(const char * name, const char ** names);
} CMAP_UTIL_PUBLIC;

extern const CMAP_UTIL_PUBLIC cmap_util_public;

#endif
