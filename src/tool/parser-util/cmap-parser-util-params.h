#ifndef __CMAP_PARSER_UTIL_PARAMS_H__
#define __CMAP_PARSER_UTIL_PARAMS_H__

typedef struct
{
  char * decl;
  char * impl;
} CMAP_PARSER_UTIL_PARAMS_RET;

typedef struct
{
  void (*clone)();
  CMAP_PARSER_UTIL_PARAMS_RET (*get)();
  void (*delete)(CMAP_PARSER_UTIL_PARAMS_RET ret);
} CMAP_PARSER_UTIL_PARAMS_PUBLIC;

extern const CMAP_PARSER_UTIL_PARAMS_PUBLIC cmap_parser_util_params_public;

#endif
