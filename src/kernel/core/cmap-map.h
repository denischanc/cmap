#ifndef __CMAP_MAP_H__
#define __CMAP_MAP_H__

extern const char * CMAP_MAP_NATURE;

typedef struct CMAP_MAP_s CMAP_MAP;

struct CMAP_MAP_s
{
  void * internal_;

  const char * (*nature)(CMAP_MAP * this);

  void (*delete)(CMAP_MAP * this);

  void (*set)(CMAP_MAP * this, const char * key, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_MAP * this, const char * key);

  void * (*new)(CMAP_MAP * this, int size);
};

void cmap_map__set(CMAP_MAP * this, const char * key, CMAP_MAP * val);
CMAP_MAP * cmap_map__get(CMAP_MAP * this, const char * key);

void * cmap_map__new(CMAP_MAP * this, int size);

CMAP_MAP * cmap_map_create();
void cmap_map_delete(CMAP_MAP * map);

#endif
