#ifndef __CMAP_PARSER_BLOCK_H__
#define __CMAP_PARSER_BLOCK_H__

typedef struct
{
  void (*if_)(char * cmp_call);
  void (*else_empty)();
  void (*else_if)();
  void (*else_)();

  void (*for_)(char * cmp_call, char * loop_call);
  void (*while_)(char * cmp_call);
} CMAP_PARSER_BLOCK_PUBLIC;

extern const CMAP_PARSER_BLOCK_PUBLIC cmap_parser_block_public;

#endif
