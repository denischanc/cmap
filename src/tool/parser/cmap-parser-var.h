#ifndef __CMAP_PARSER_VAR_H__
#define __CMAP_PARSER_VAR_H__

char * cmap_parser_var_path(char * map, char * name);
char * cmap_parser_var_name(char * name);

void cmap_parser_var_set_local(char * name, char * map);
char * cmap_parser_var_set_fn_arg_name(const char * name, int off);
void cmap_parser_var_set_path(char * src, char * name, char * map);
void cmap_parser_var_set_global(char * name, char * map);

void cmap_parser_var_set_sb_int(char * map, char * i, char * map_src);
void cmap_parser_var_set_sb_string(char * map, char * string, char * map_src);
void cmap_parser_var_set_sb_map(char * map, char * map_i, char * map_src);
char * cmap_parser_var_sb_int(char * map, char * i);
char * cmap_parser_var_sb_string(char * map, char * string);
char * cmap_parser_var_sb_map(char * map, char * map_i);

#endif
