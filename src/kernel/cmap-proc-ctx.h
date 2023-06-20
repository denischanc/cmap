#ifndef __CMAP_PROC_CTX_H__
#define __CMAP_PROC_CTX_H__

#include "cmap-proc-ctx-type.h"
#include "cmap-scanner.h"
#include "cmap-list-type.h"
#include "cmap-env-type.h"
#include "cmap-aislestore.h"
#include "cmap-prototypestore.h"
#include "cmap-pool.h"

struct CMAP_PROC_CTX
{
  void * internal;

  CMAP_MAP * ret;

  void (*delete)(CMAP_PROC_CTX * this);

  CMAP_ENV * (*env)(CMAP_PROC_CTX * this);
  CMAP_AISLESTORE * (*aislestore)(CMAP_PROC_CTX * this);
  CMAP_PROTOTYPESTORE * (*prototypestore)(CMAP_PROC_CTX * this);
  CMAP_POOL_LIST * (*pool_list)(CMAP_PROC_CTX * this);
  CMAP_POOL_STRING * (*pool_string)(CMAP_PROC_CTX * this);
  CMAP_POOL_INT * (*pool_int)(CMAP_PROC_CTX * this);
  CMAP_MAP * (*global_env)(CMAP_PROC_CTX * this);

  void (*push_scanner)(CMAP_PROC_CTX * this);
  void (*pop_scanner)(CMAP_PROC_CTX * this);
  yyscan_t (*scanner)(CMAP_PROC_CTX * this);

  void (*push_local_stack)(CMAP_PROC_CTX * this);
  void (*pop_local_stack)(CMAP_PROC_CTX * this);
  CMAP_LIST * (*local_stack)(CMAP_PROC_CTX * this);

  void (*push_definitions)(CMAP_PROC_CTX * this);
  void (*pop_definitions)(CMAP_PROC_CTX * this);
  CMAP_MAP * (*definitions)(CMAP_PROC_CTX * this);
};

typedef struct
{
  CMAP_PROC_CTX * (*create)(CMAP_ENV * env);
} CMAP_PROC_CTX_PUBLIC;

extern const CMAP_PROC_CTX_PUBLIC cmap_proc_ctx_public;

#endif
