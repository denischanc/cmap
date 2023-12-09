#ifndef __CMAP_PROC_CTX_H__
#define __CMAP_PROC_CTX_H__

#include "cmap-proc-ctx-type.h"
#include "cmap-list-type.h"
#include "cmap-env-type.h"
#include "cmap-aislestore.h"
#include "cmap-prototypestore.h"
#include "cmap-pool.h"

struct CMAP_PROC_CTX
{
  void * internal;

  void (*delete)(CMAP_PROC_CTX * this);

  CMAP_ENV * (*env)(CMAP_PROC_CTX * this);
  CMAP_AISLESTORE * (*aislestore)(CMAP_PROC_CTX * this);
  CMAP_PROTOTYPESTORE * (*prototypestore)(CMAP_PROC_CTX * this);
  CMAP_POOL_LIST * (*pool_list)(CMAP_PROC_CTX * this);
  CMAP_POOL_STRING * (*pool_string)(CMAP_PROC_CTX * this);
  CMAP_POOL_INT * (*pool_int)(CMAP_PROC_CTX * this);
  CMAP_MAP * (*global_env)(CMAP_PROC_CTX * this);

  void (*push_local)(CMAP_PROC_CTX * this);
  CMAP_MAP * (*local_definitions)(CMAP_PROC_CTX * this);
  CMAP_LIST * (*local_stack)(CMAP_PROC_CTX * this);
  CMAP_MAP * (*pop_local)(CMAP_PROC_CTX * this, CMAP_MAP * ret);
};

typedef struct
{
  CMAP_PROC_CTX * (*create)(CMAP_ENV * env);
} CMAP_PROC_CTX_PUBLIC;

extern const CMAP_PROC_CTX_PUBLIC cmap_proc_ctx_public;

#endif
