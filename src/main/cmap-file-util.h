#ifndef __CMAP_FILE_UTIL_H__
#define __CMAP_FILE_UTIL_H__

typedef struct
{
  int (*to_file)(const char * path, const char * txt);
} CMAP_FILE_UTIL_PUBLIC;

extern const CMAP_FILE_UTIL_PUBLIC cmap_file_util_public;

#endif
