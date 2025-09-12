#ifndef __CMAP_PARSER_CREATOR_H__
#define __CMAP_PARSER_CREATOR_H__

typedef struct
{
  char * (*map_args)(char * args);
  char * (*list_args)(char * args);
  char * (*string)(char * string);
  char * (*int_)(char * i);

  char * (*new)(char * map, char * args);
} CMAP_PARSER_CREATOR_PUBLIC;

extern const CMAP_PARSER_CREATOR_PUBLIC cmap_parser_creator_public;

#endif
