#ifndef __CMAP_FILE_UTIL_H__
#define __CMAP_FILE_UTIL_H__

char cmap_file_util_to_file(const char * path, const char * txt, ...);

char * cmap_file_util_dirname(const char * path);
const char * cmap_file_util_basename(const char * path);
char * cmap_file_util_basename_no_ext(const char * path);
const char * cmap_file_util_extension(const char * path);

#endif
