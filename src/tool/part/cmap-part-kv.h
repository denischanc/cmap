#ifndef __CMAP_PART_KV_H__
#define __CMAP_PART_KV_H__

#include "cmap-stack-define.h"

CMAP_STACK_TYPE(PART_KV)
typedef CMAP_STACK_PART_KV CMAP_PART_KV;

typedef void (*CMAP_PART_KV_APPLY)(const char * map, const char * name,
  const char * map_name, char dirty, void * data);

typedef struct
{
  void (*put)(CMAP_PART_KV ** kv_ptr, const char * map, const char * name,
    const char * map_name);
  const char * (*get)(CMAP_PART_KV * kv, const char * map, const char * name);

  void (*dirty)(CMAP_PART_KV * kv, const char * map, const char * name);
  char (*is_dirty_n_rst)(CMAP_PART_KV * kv, const char * map,
    const char * name);

  void (*apply)(CMAP_PART_KV * kv, CMAP_PART_KV_APPLY fn, void * data);

  void (*delete_key)(CMAP_PART_KV ** kv_ptr, const char * map,
    const char * name);
  void (*delete)(CMAP_PART_KV ** kv_ptr);
} CMAP_PART_KV_PUBLIC;

extern const CMAP_PART_KV_PUBLIC cmap_part_kv_public;

#endif
