#ifndef __CMAP_MAP_H__
#define __CMAP_MAP_H__

#include "cmap-core.h"

extern const char * CMAP_MAP_NATURE;

struct CMAP_MAP_s
{
  void * internal_;

  const char * (*nature)(CMAP_MAP * this);

  void (*delete)(CMAP_MAP * this);

  void (*set)(CMAP_MAP * this, const char * key, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_MAP * this, const char * key);

  void * (*new)(CMAP_MAP * this, int size);

  char (*is_key)(CMAP_MAP * this, const char * key);
  void (*keys)(CMAP_MAP * this, CMAP_LIST * keys);
};

void cmap_map__set(CMAP_MAP * this, const char * key, CMAP_MAP * val);
CMAP_MAP * cmap_map__get(CMAP_MAP * this, const char * key);

void * cmap_map__new(CMAP_MAP * this, int size);

char cmap_map__is_key(CMAP_MAP * this, const char * key);
void cmap_map__keys(CMAP_MAP * this, CMAP_LIST * keys);

CMAP_MAP * cmap_map_create();
CMAP_MAP * cmap_root_map_create();
void cmap_map_delete(CMAP_MAP * map);

#endif
