#ifndef __CMAP_MAP_H__
#define __CMAP_MAP_H__

#include "cmap-core.h"
#include "cmap-map-define.h"

typedef void (*CMAP_MAP_ENTRY_FN)(const char * key, CMAP_MAP ** val,
  void * data);

typedef struct
{
  const char * nature;

  CMAP_MAP * (*create)(const char * aisle);
  CMAP_MAP * (*create_root)(const char * aisle);
  void (*init)(CMAP_MAP * map);
  CMAP_MAP * (*delete)(CMAP_MAP * map);

  void (*set)(CMAP_MAP * this, const char * key, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_MAP * this, const char * key);

  void * (*new)(CMAP_MAP * this, int size, const char * aisle);

  char (*is_key)(CMAP_MAP * this, const char * key);
  void (*keys)(CMAP_MAP * this, CMAP_LIST * keys, const char * aisle);

  void (*apply)(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data);
} CMAP_MAP_PUBLIC;

extern const CMAP_MAP_PUBLIC cmap_map_public;

struct CMAP_MAP_s
{
  void * internal;

  const char * (*nature)(CMAP_MAP * this);

  CMAP_MAP * (*delete)(CMAP_MAP * this);

  void (*set)(CMAP_MAP * this, const char * key, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_MAP * this, const char * key);

  void * (*new)(CMAP_MAP * this, int size, const char * aisle);

  char (*is_key)(CMAP_MAP * this, const char * key);
  void (*keys)(CMAP_MAP * this, CMAP_LIST * keys, const char * aisle);

  void (*apply)(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data);
};

#endif
