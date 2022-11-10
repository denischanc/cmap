#ifndef __CMAP_CTX_H__
#define __CMAP_CTX_H__

#include "cmap-aislestore.h"
#include "cmap-pool.h"

typedef struct CMAP_CTX_s CMAP_CTX;

struct CMAP_CTX_s
{
  void * internal;

  void (*delete)(CMAP_CTX * this);

  CMAP_AISLESTORE * (*aislestore)(CMAP_CTX * this);

  CMAP_POOL_LIST * (*pool_list)(CMAP_CTX * this);
  CMAP_POOL_STRING * (*pool_string)(CMAP_CTX * this);

  CMAP_MAP * (*global_env)(CMAP_CTX * this);
};

typedef struct
{
  CMAP_CTX * (*create)();
} CMAP_CTX_PUBLIC;

extern const CMAP_CTX_PUBLIC cmap_ctx_public;

#endif
