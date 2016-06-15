#ifndef __CMAP_MAP_H__
#define __CMAP_MAP_H__

#include "cmap-core.h"

extern const char * CMAP_MAP_NATURE;

typedef void (*CMAP_MAP_ENTRY_FN)(const char * key, CMAP_MAP ** val,
  void * data);

struct CMAP_MAP_s
{
  void * internal_;

  const char * (*nature)(CMAP_MAP * this);

  CMAP_MAP * (*delete)(CMAP_MAP * this);

  void (*set)(CMAP_MAP * this, const char * key, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_MAP * this, const char * key);

  void * (*new)(CMAP_MAP * this, int size, const char * aisle);

  char (*is_key)(CMAP_MAP * this, const char * key);
  void (*keys)(CMAP_MAP * this, CMAP_LIST * keys, const char * aisle);

  void (*apply)(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data);
};

void cmap_map__set(CMAP_MAP * this, const char * key, CMAP_MAP * val);
CMAP_MAP * cmap_map__get(CMAP_MAP * this, const char * key);

void * cmap_map__new(CMAP_MAP * this, int size, const char * aisle);

char cmap_map__is_key(CMAP_MAP * this, const char * key);
void cmap_map__keys(CMAP_MAP * this, CMAP_LIST * keys, const char * aisle);

void cmap_map__apply(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data);

CMAP_MAP * cmap_map_create(const char * aisle);
CMAP_MAP * cmap_root_map_create(const char * aisle);
void cmap_map_init(CMAP_MAP * map);
CMAP_MAP * cmap_map_delete(CMAP_MAP * map);

#endif
