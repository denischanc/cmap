#ifndef __CMAP_PARSER_STRING_H__
#define __CMAP_PARSER_STRING_H__

typedef struct
{
  char * (*create)(const char * txt);
  char * (*create_args)(const char * txt, ...);

  void (*append)(char ** src, const char * txt);
  void (*append_args)(char ** src, const char * txt, ...);
} CMAP_PARSER_STRING_PUBLIC;

extern const CMAP_PARSER_STRING_PUBLIC cmap_parser_string_public;

#endif
