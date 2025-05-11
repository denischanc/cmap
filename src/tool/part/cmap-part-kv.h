#ifndef __CMAP_PART_KV_H__
#define __CMAP_PART_KV_H__

#include "cmap-stack-define.h"

CMAP_STACK_TYPE(PART_KV)
typedef CMAP_STACK_PART_KV CMAP_PART_KV;

typedef struct
{
  void (*put)(CMAP_PART_KV ** kv_ptr, const char * key, const char * val);
  const char * (*get)(CMAP_PART_KV * kv, const char * key);

  void (*dirty)(CMAP_PART_KV * kv, const char * key);
  char (*is_dirty_n_rst)(CMAP_PART_KV * kv, const char * key);

  void (*delete_key)(CMAP_PART_KV ** kv_ptr, const char * key);
  void (*delete)(CMAP_PART_KV ** kv_ptr);
} CMAP_PART_KV_PUBLIC;

extern const CMAP_PART_KV_PUBLIC cmap_part_kv_public;

#endif
