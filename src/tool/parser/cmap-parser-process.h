#ifndef __CMAP_PARSER_PROCESS_H__
#define __CMAP_PARSER_PROCESS_H__

typedef struct
{
  char * (*process)(char * map, char * fn_name, char * args);
  char * (*process_fn)(char * fn, char * args);
  char * (*process_instruction)(char * txt, char need_ret);
  char * (*process_c)(char * fn_name, char need_ret);

  void (*return_)(char * map);
} CMAP_PARSER_PROCESS_PUBLIC;

extern const CMAP_PARSER_PROCESS_PUBLIC cmap_parser_process_public;

#endif
