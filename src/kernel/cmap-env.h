#ifndef __CMAP_ENV_H__
#define __CMAP_ENV_H__

#include "cmap-env-type.h"
#include "cmap-map-type.h"
#include "cmap-aislestore.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-pool.h"

struct CMAP_ENV_s
{
  void * internal;

  void (*delete)(CMAP_ENV * this);

  CMAP_MAP * (*main)(CMAP_ENV * this, int argc, char * argv[],
    CMAP_MAP * definitions, const char * impl);

  CMAP_AISLESTORE * (*aislestore)(CMAP_ENV * this);

  CMAP_POOL_LIST * (*pool_list)(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx);
  CMAP_POOL_STRING * (*pool_string)(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx);
  CMAP_POOL_INT * (*pool_int)(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx);

  CMAP_MAP * (*global)(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx);
};

typedef struct
{
  CMAP_ENV * (*create)();
  void (*delete_all)();
} CMAP_ENV_PUBLIC;

extern const CMAP_ENV_PUBLIC cmap_env_public;

#endif
