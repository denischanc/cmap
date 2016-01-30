#ifndef __CMAP_MAP_H__
#define __CMAP_MAP_H__

typedef struct CMAP_MAP_s CMAP_MAP;

struct CMAP_MAP_s
{
  void * internal_;

  void (*delete)(CMAP_MAP * this);

  void (*add)(CMAP_MAP * this, const char * key, CMAP_MAP * val);
};

CMAP_MAP * cmap_map_create();
void cmap_map_init(CMAP_MAP * map);
void cmap_map_delete(CMAP_MAP * map);

#endif
