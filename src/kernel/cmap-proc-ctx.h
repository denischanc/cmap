#ifndef __CMAP_PROC_CTX_H__
#define __CMAP_PROC_CTX_H__

#include "cmap-proc-ctx-type.h"
#include "cmap-env-type.h"
#include "cmap-map-type.h"
#include "cmap-prototypestore.h"
#include "cmap-pool.h"

#define CMAP_PROC_CTX_POOL_DECL(TYPE, type) \
  CMAP_POOL_##TYPE * (*pool_##type)(CMAP_PROC_CTX * this);

struct CMAP_PROC_CTX
{
  CMAP_MAP * (*delete)(CMAP_PROC_CTX * this, CMAP_MAP * ret);

  CMAP_ENV * (*env)(CMAP_PROC_CTX * this);
  CMAP_PROTOTYPESTORE * (*prototypestore)(CMAP_PROC_CTX * this);
  CMAP_POOL_LOOP(CMAP_PROC_CTX_POOL_DECL)
  CMAP_MAP * (*global_env)(CMAP_PROC_CTX * this);

  CMAP_MAP * (*local_definitions)(CMAP_PROC_CTX * this);

  void (*local_refs_add)(CMAP_PROC_CTX * this, CMAP_LIFECYCLE * lc,
    char created);
  void (*local_refs_rm)(CMAP_PROC_CTX * this, CMAP_LIFECYCLE * lc);

  CMAP_PROC_CTX * (*new_level)(CMAP_PROC_CTX * this);
};

typedef struct
{
  CMAP_PROC_CTX * (*create)(CMAP_ENV * env);
} CMAP_PROC_CTX_PUBLIC;

extern const CMAP_PROC_CTX_PUBLIC cmap_proc_ctx_public;

#endif
