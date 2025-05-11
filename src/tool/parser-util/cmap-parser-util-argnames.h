#ifndef __CMAP_PARSER_UTIL_ARGNAMES_H__
#define __CMAP_PARSER_UTIL_ARGNAMES_H__

typedef struct
{
  char * (*get)();
} CMAP_PARSER_UTIL_ARGNAMES_PUBLIC;

extern const CMAP_PARSER_UTIL_ARGNAMES_PUBLIC cmap_parser_util_argnames_public;

#endif
