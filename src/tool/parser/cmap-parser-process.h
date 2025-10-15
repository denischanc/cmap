#ifndef __CMAP_PARSER_PROCESS_H__
#define __CMAP_PARSER_PROCESS_H__

typedef struct
{
  char * (*process)(char * map, char * fn_name, char * args, char do_return);
  char * (*process_fn)(char * fn, char * args, char do_return);

  char * (*process_c)(char * fn_name, char do_return);
  char (*import)(char ** map_name, char * path, char * fn_name);

  void (*return_)(char * map);
} CMAP_PARSER_PROCESS_PUBLIC;

extern const CMAP_PARSER_PROCESS_PUBLIC cmap_parser_process_public;

#endif
