#ifndef __CMAP_PART_KV_H__
#define __CMAP_PART_KV_H__

#include "cmap-stack-define.h"

CMAP_STACK_TYPE(PART_KV)
typedef CMAP_STACK_PART_KV CMAP_PART_KV;

typedef char (*CMAP_PART_KV_APPLY)(const char * map, const char * name,
  const char * map_name, void * data);

void cmap_part_kv_put(CMAP_PART_KV ** kv_ptr, const char * map,
  const char * name, const char * map_name);
const char * cmap_part_kv_get(CMAP_PART_KV * kv, const char * map,
  const char * name);

void cmap_part_kv_apply(CMAP_PART_KV ** kv_ptr, CMAP_PART_KV_APPLY fn,
  void * data);

void cmap_part_kv_delete_key(CMAP_PART_KV ** kv_ptr, const char * map,
    const char * name);
void cmap_part_kv_delete(CMAP_PART_KV ** kv_ptr);

#endif
