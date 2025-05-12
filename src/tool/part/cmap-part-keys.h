#ifndef __CMAP_PART_KEYS_H__
#define __CMAP_PART_KEYS_H__

#include "cmap-part-kv.h"

typedef CMAP_PART_KV CMAP_PART_KEYS;

typedef void (*CMAP_PART_KEYS_APPLY)(const char * map, const char * name,
  void * data);

typedef struct
{
  void (*add)(CMAP_PART_KEYS ** keys, const char * map, const char * name);

  void (*apply)(CMAP_PART_KEYS * keys, CMAP_PART_KEYS_APPLY fn, void * data);

  void (*delete)(CMAP_PART_KEYS ** keys);
} CMAP_PART_KEYS_PUBLIC;

extern const CMAP_PART_KEYS_PUBLIC cmap_part_keys_public;

#endif
