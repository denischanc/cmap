#ifndef __CMAP_JOB_H__
#define __CMAP_JOB_H__

#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

typedef struct
{
  CMAP_MAP * (*create)(CMAP_PROC_CTX * proc_ctx);
} CMAP_JOB_PUBLIC;

extern const CMAP_JOB_PUBLIC cmap_job_public;

#endif
