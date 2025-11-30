#ifndef __CMAP_OP_HANDLER_H__
#define __CMAP_OP_HANDLER_H__

#include "cmap-op-handler-define.h"
#include "cmap-proc-ctx-type.h"
#include "cmap-op.h"

typedef struct
{
  char (*match)(const char * nature_l, const char * nature_r);

  CMAP_MAP * (*op)(CMAP_MAP * map_l, CMAP_MAP * map_r,
    CMAP_PROC_CTX * proc_ctx);
} CMAP_OP_HANDLER;

typedef struct
{
  char (*match)(const char * nature_dst, const char * nature_src);

  void (*op)(CMAP_MAP * map_dst, CMAP_MAP * map_src);
} CMAP_OP_HANDLER_SELF;

typedef struct
{
  char (*match)(const char * nature);

  void (*op)(CMAP_MAP * map);
} CMAP_OP_HANDLER_UNIQUE;

CMAP_OP_LOOP(CMAP_OP_HANDLER_DEFINE)
CMAP_OP_SELF_LOOP(CMAP_OP_HANDLER_SELF_DEFINE)
CMAP_OP_UNIQUE_LOOP(CMAP_OP_HANDLER_UNIQUE_DEFINE)

#endif
