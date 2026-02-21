#ifndef __CMAP_PARSER_CREATOR_H__
#define __CMAP_PARSER_CREATOR_H__

char * cmap_parser_creator_map_args(char * args);
char * cmap_parser_creator_list_args(char * args);
char * cmap_parser_creator_string(char * string);
char * cmap_parser_creator_int(char * i);

char * cmap_parser_creator_new(char * map, char * args);

#endif
