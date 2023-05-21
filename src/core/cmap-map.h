#ifndef __CMAP_MAP_H__
#define __CMAP_MAP_H__

#include "cmap-map-type.h"
#include "cmap-map-define.h"
#include "cmap-map-ext.h"
#include "cmap-lifecycle.h"

typedef void (*CMAP_MAP_ENTRY_FN)(const char * key, CMAP_MAP ** val,
  void * data);

struct CMAP_MAP_s
{
  CMAP_LIFECYCLE super;

  void * internal;

  const char * (*nature)(CMAP_MAP * this);

  void (*set)(CMAP_MAP * this, const char * key, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_MAP * this, const char * key);

  void * (*new)(CMAP_MAP * this, int size, CMAP_PROC_CTX * proc_ctx,
    const char * aisle);

  char (*is_key)(CMAP_MAP * this, const char * key);
  void (*keys)(CMAP_MAP * this, CMAP_LIST * keys, CMAP_PROC_CTX * proc_ctx,
    const char * aisle);

  void (*apply)(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data);
};

typedef struct
{
  CMAP_MAP * (*create)(CMAP_PROC_CTX * proc_ctx, const char * aisle);
  CMAP_MAP * (*create_root)(CMAP_PROC_CTX * proc_ctx, const char * aisle);
  void (*init)(CMAP_MAP * map, CMAP_PROC_CTX * proc_ctx, const char * aisle);
  CMAP_LIFECYCLE * (*delete)(CMAP_MAP * map);

  void (*set)(CMAP_MAP * this, const char * key, CMAP_MAP * val);
  CMAP_MAP * (*get)(CMAP_MAP * this, const char * key);

  void * (*new)(CMAP_MAP * map, int size, CMAP_PROC_CTX * proc_ctx,
    const char * aisle);

  char (*is_key)(CMAP_MAP * this, const char * key);
  void (*keys)(CMAP_MAP * this, CMAP_LIST * keys, CMAP_PROC_CTX * proc_ctx,
    const char * aisle);

  void (*apply)(CMAP_MAP * this, CMAP_MAP_ENTRY_FN fn, void * data);
} CMAP_MAP_PUBLIC;

extern const CMAP_MAP_PUBLIC cmap_map_public;

#endif
