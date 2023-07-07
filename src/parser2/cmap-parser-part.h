#ifndef __CMAP_PARSER_PART_H__
#define __CMAP_PARSER_PART_H__

typedef struct
{
  char ** (*includes)();
  char ** (*functions)();
  char ** (*main)();
  char ** (*instructions)();
  char ** (*definitions)();

  void (*return_)();
  char (*is_return)();

  void (*push_instructions)();
  char * (*pop_instructions)();
} CMAP_PARSER_PART_PUBLIC;

extern const CMAP_PARSER_PART_PUBLIC cmap_parser_part_public;

#endif
