#ifndef __CMAP_PART_THIS_ARGS_H__
#define __CMAP_PART_THIS_ARGS_H__

typedef struct
{
  void (*add)();
  char (*is)(const char * map, const char * name);
  void (*clean)();
} CMAP_PART_THIS_ARGS_PUBLIC;

extern const CMAP_PART_THIS_ARGS_PUBLIC cmap_part_this_args_public;

#endif
