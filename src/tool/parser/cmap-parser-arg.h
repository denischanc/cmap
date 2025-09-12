#ifndef __CMAP_PARSER_ARG_H__
#define __CMAP_PARSER_ARG_H__

typedef struct
{
  char * (*args)(char * map);
  char * (*args_push)(char * list, char * map);

  void (*arg_name)(char * name);

  char * (*args_map)(char * name, char * map);
  char * (*args_map_push)(char * list, char * name, char * map);

  char * (*names)(char * names, char * name);
} CMAP_PARSER_ARG_PUBLIC;

extern const CMAP_PARSER_ARG_PUBLIC cmap_parser_arg_public;

#endif
