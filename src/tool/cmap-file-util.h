#ifndef __CMAP_FILE_UTIL_H__
#define __CMAP_FILE_UTIL_H__

typedef struct
{
  char (*to_file)(const char * path, const char * txt, ...);

  char * (*dirname)(const char * path);
  const char * (*basename)(const char * path);
  char * (*basename_no_ext)(const char * path);
  const char * (*extension)(const char * path);
} CMAP_FILE_UTIL_PUBLIC;

extern const CMAP_FILE_UTIL_PUBLIC cmap_file_util_public;

#endif
