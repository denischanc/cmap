#ifndef __CMAP_LOG_H__
#define __CMAP_LOG_H__

#include <stdarg.h>
#include "cmap-log-define.h"

typedef struct
{
  void (*log)(const char * level, const char * msg, ...);
  void (*vlog)(const char * level, const char * msg, va_list ap);
} CMAP_LOG;

typedef struct
{
  CMAP_LOG * (*init)();
  CMAP_LOG * (*instance)();

  CMAP_LOG_LOOP(CMAP_LOG_FN)
} CMAP_LOG_PUBLIC;

CMAP_LOG_LOOP(CMAP_LOG_LEVEL)

extern const CMAP_LOG_PUBLIC cmap_log_public;

#endif
