#ifndef __CMAP_PARSER_VAR_H__
#define __CMAP_PARSER_VAR_H__

typedef struct
{
  char * (*path)(char * map, char * name);
  char * (*name)(char * name);

  void (*set_local)(char * name, char * map);
  void (*set_fn_arg_name)(char * name, int off);
  void (*set_path)(char * src, char * name, char * map);
  void (*set_global)(char * name, char * map);

  void (*set_sb_int)(char * map, char * i, char * map_src);
  void (*set_sb_string)(char * map, char * string, char * map_src);
  void (*set_sb_map)(char * map, char * map_i, char * map_src);
  char * (*sb_int)(char * map, char * i);
  char * (*sb_string)(char * map, char * string);
  char * (*sb_map)(char * map, char * map_i);
} CMAP_PARSER_VAR_PUBLIC;

extern const CMAP_PARSER_VAR_PUBLIC cmap_parser_var_public;

#endif
