#ifndef __CMAP_DO_PARSE_H__
#define __CMAP_DO_PARSE_H__

typedef struct
{
  const char * (*path)();

  char (*parse)(const char * path);
} CMAP_DO_PARSE_PUBLIC;

extern const CMAP_DO_PARSE_PUBLIC cmap_do_parse_public;

#endif
