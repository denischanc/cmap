#ifndef __CMAP_LOG_H__
#define __CMAP_LOG_H__

#include <cmap/log.h>
#include "cmap-log-define.h"

typedef struct
{
  CMAP_LOG * (*init)();
  CMAP_LOG * (*instance)();

  CMAP_LOG_LOOP(CMAP_LOG_FN)
} CMAP_LOG_PUBLIC;

CMAP_LOG_LOOP(CMAP_LOG_LEVEL)

extern const CMAP_LOG_PUBLIC cmap_log_public;

#endif
