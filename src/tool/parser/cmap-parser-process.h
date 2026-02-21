#ifndef __CMAP_PARSER_PROCESS_H__
#define __CMAP_PARSER_PROCESS_H__

char * cmap_parser_process(char * map, char * fn_name, char * args,
  char do_return);
char * cmap_parser_process_fn(char * fn, char * args, char do_return);

char * cmap_parser_process_c(char * fn_name, char do_return);
char cmap_parser_process_import(char ** map_name, char * path, char * fn_name);

void cmap_parser_process_return(char * map);

#endif
