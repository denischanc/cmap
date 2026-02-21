#ifndef __CMAP_PARSER_ARG_H__
#define __CMAP_PARSER_ARG_H__

char * cmap_parser_arg_args(char * map);
char * cmap_parser_arg_args_push(char * list, char * map);

void cmap_parser_arg_arg_name(char * name);

char * cmap_parser_arg_args_map(char * name, char * map);
char * cmap_parser_arg_args_map_push(char * list, char * name, char * map);

char * cmap_parser_arg_names(char * names, char * name);

#endif
