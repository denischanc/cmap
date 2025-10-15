#ifndef __CMAP_BUILD_H__
#define __CMAP_BUILD_H__

#define CMAP_BUILD_MODULE_NAME "build"

typedef struct
{
  void (*set_parse_path)(const char * path);
  const char * (*get_parse_path)();
  int (*parse)();

  int (*main)(int argc, char * argv[]);
} CMAP_BUILD_PUBLIC;

extern const CMAP_BUILD_PUBLIC cmap_build_public;

#endif
