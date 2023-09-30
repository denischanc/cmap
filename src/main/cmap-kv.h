#ifndef __CMAP_KV_H__
#define __CMAP_KV_H__

typedef struct CMAP_KV CMAP_KV;

typedef struct
{
  CMAP_KV * (*create)();
  void (*put)(CMAP_KV * kv, const char * key, const char * val);
  const char * (*get)(CMAP_KV * kv, const char * key);
  void (*delete)(CMAP_KV * kv);
} CMAP_KV_PUBLIC;

extern const CMAP_KV_PUBLIC cmap_kv_public;

#endif
