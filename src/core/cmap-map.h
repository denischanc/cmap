#ifndef __CMAP_MAP_H__
#define __CMAP_MAP_H__

#include "cmap-map-define.h"
#include "cmap-map-type.h"
#include "cmap-map-int.h"
#include "cmap-lifecycle.h"

typedef void (*CMAP_MAP_ENTRY_FN)(const char * key, CMAP_MAP ** val,
  void * data, CMAP_PROC_CTX * proc_ctx);

struct CMAP_MAP
{
  CMAP_LIFECYCLE super;

  CMAP_MAP_INTERNAL internal;
};

void cmap_map_nested(CMAP_LIFECYCLE * lc, CMAP_SLIST_LC_PTR * list,
  CMAP_PROC_CTX * proc_ctx);

CMAP_MAP * cmap_map_set(CMAP_MAP * map, const char * key, CMAP_MAP * val,
  CMAP_PROC_CTX * proc_ctx);
CMAP_MAP * cmap_map_get(CMAP_MAP * map, const char * key);

CMAP_MAP * cmap_map_new(CMAP_MAP * map, CMAP_PROC_CTX * proc_ctx);

char cmap_map_has(CMAP_MAP * map, const char * key);
CMAP_LIST * cmap_map_keys(CMAP_MAP * map, CMAP_PROC_CTX * proc_ctx);

void cmap_map_apply(CMAP_MAP * map, CMAP_MAP_ENTRY_FN fn, void * data,
  CMAP_PROC_CTX * proc_ctx);

void cmap_map_clean(CMAP_MAP * map, CMAP_PROC_CTX * proc_ctx);

void cmap_map_ghost(CMAP_MAP * map);
char cmap_map_is_ghost(CMAP_MAP * map);

void cmap_map_delete(CMAP_LIFECYCLE * lc, CMAP_PROC_CTX * proc_ctx);
CMAP_MAP * cmap_map_init(CMAP_MAP * map, CMAP_INITARGS * initargs);
CMAP_MAP * cmap_map_create_root(CMAP_PROC_CTX * proc_ctx);
CMAP_MAP * cmap_map_create(CMAP_PROC_CTX * proc_ctx);

#endif
