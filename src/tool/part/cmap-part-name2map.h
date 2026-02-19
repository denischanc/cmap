#ifndef __CMAP_PART_NAME2MAP_H__
#define __CMAP_PART_NAME2MAP_H__

typedef struct
{
  char * map, new, affected;
} CMAP_PART_NAME2MAP_RET;

void cmap_part_name2map_put(const char * map, const char * name,
  const char * next_name);

CMAP_PART_NAME2MAP_RET cmap_part_name2map_get(const char * map,
  const char * name, char * next_name);

void cmap_part_name2map_clean_after_proc();

#endif
