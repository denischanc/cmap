#ifndef __CMAP_ENV_H__
#define __CMAP_ENV_H__

#include "cmap-env-type.h"
#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-pool.h"
#include "cmap-prototypestore.h"

struct CMAP_ENV
{
  int argc;
  char ** argv;

  void (*delete)(CMAP_ENV * this);

  void (*push_proc_ctx)(CMAP_ENV * this, CMAP_PROC_CTX * cur_proc_ctx);
  CMAP_PROC_CTX * (*proc_ctx)(CMAP_ENV * this);
  void (*pop_proc_ctx)(CMAP_ENV * this);

  CMAP_PROTOTYPESTORE * (*prototypestore)(CMAP_ENV * this,
    CMAP_PROC_CTX * proc_ctx);

  CMAP_POOL_LIST * (*pool_list)(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx);
  CMAP_POOL_STRING * (*pool_string)(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx);
  CMAP_POOL_INT * (*pool_int)(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx);

  CMAP_MAP * (*global)(CMAP_ENV * this, CMAP_PROC_CTX * proc_ctx);
};

typedef struct
{
  CMAP_ENV * (*create)(int argc, char ** argv);
  void (*delete_all)();
} CMAP_ENV_PUBLIC;

extern const CMAP_ENV_PUBLIC cmap_env_public;

#endif
