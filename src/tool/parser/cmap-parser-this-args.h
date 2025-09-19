#ifndef __CMAP_PARSER_THIS_ARGS_H__
#define __CMAP_PARSER_THIS_ARGS_H__

typedef struct
{
  char (*is)(const char * map, const char * name);

  const char * (*type)(const char * name);

  const char * (*decl)();
} CMAP_PARSER_THIS_ARGS_PUBLIC;

extern const CMAP_PARSER_THIS_ARGS_PUBLIC cmap_parser_this_args_public;

#endif
