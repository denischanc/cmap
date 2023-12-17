#ifndef __CMAP_PROC_CTX_H__
#define __CMAP_PROC_CTX_H__

#include "cmap-proc-ctx-type.h"
#include "cmap-env-type.h"
#include "cmap-map-type.h"
#include "cmap-prototypestore.h"
#include "cmap-pool.h"

struct CMAP_PROC_CTX
{
  void * internal;

  CMAP_MAP * (*delete)(CMAP_PROC_CTX * this, CMAP_MAP * ret);

  CMAP_ENV * (*env)(CMAP_PROC_CTX * this);
  CMAP_PROTOTYPESTORE * (*prototypestore)(CMAP_PROC_CTX * this);
  CMAP_POOL_LIST * (*pool_list)(CMAP_PROC_CTX * this);
  CMAP_POOL_STRING * (*pool_string)(CMAP_PROC_CTX * this);
  CMAP_POOL_INT * (*pool_int)(CMAP_PROC_CTX * this);
  CMAP_MAP * (*global_env)(CMAP_PROC_CTX * this);

  CMAP_MAP * (*local_definitions)(CMAP_PROC_CTX * this);
  void (*local_refs_add)(CMAP_PROC_CTX * this, CMAP_LIFECYCLE * lc,
    char created);
};

typedef struct
{
  CMAP_PROC_CTX * (*create)(CMAP_ENV * env);
} CMAP_PROC_CTX_PUBLIC;

extern const CMAP_PROC_CTX_PUBLIC cmap_proc_ctx_public;

#endif
