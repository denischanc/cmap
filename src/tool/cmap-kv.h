#ifndef __CMAP_KV_H__
#define __CMAP_KV_H__

#include "cmap-stack-define.h"

CMAP_STACK_TYPE(kv)
typedef CMAP_STACK_kv CMAP_KV;

typedef struct
{
  void (*put)(CMAP_KV ** kv_ptr, const char * key, const char * val);
  const char * (*get)(CMAP_KV * kv, const char * key);
  void (*delete)(CMAP_KV ** kv_ptr);
} CMAP_KV_PUBLIC;

extern const CMAP_KV_PUBLIC cmap_kv_public;

#endif
