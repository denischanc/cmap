#ifndef __CMAP_LOG_H__
#define __CMAP_LOG_H__

#include "cmap-log-ext.h"
#include "cmap-log-define.h"

typedef struct
{
  CMAP_LOG * (*instance)();

  void (*log)(char lvl, const char * msg, ...);
  void (*vlog)(char lvl, const char * msg, va_list args);

  CMAP_LOG_LOOP(CMAP_LOG_DECL)
} CMAP_LOG_PUBLIC;

extern const CMAP_LOG_PUBLIC cmap_log_public;

#endif
