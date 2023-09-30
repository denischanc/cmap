#ifndef __CMAP_KV_H__
#define __CMAP_KV_H__

typedef struct KV KV;

KV * cmap_kv_create();
void cmap_kv_put(KV * kv, const char * key, const char * val);
const char * cmap_kv_get(KV * kv, const char * key);
void cmap_kv_delete(KV * kv);

#endif
