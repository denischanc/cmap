#ifndef __CMAP_STRING_H__
#define __CMAP_STRING_H__

typedef struct
{
  void (*append)(char ** src, const char * txt);
  void (*append_args)(char ** src, const char * txt, ...);
} CMAP_STRING_PUBLIC;

extern const CMAP_STRING_PUBLIC cmap_string_public;

#endif
