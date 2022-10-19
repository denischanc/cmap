#ifndef __CMAP_LOG_H__
#define __CMAP_LOG_H__

#include <stdarg.h>

typedef struct
{
  void (*debug)(const char * msg, ...);
  void (*info)(const char * msg, ...);
  void (*warn)(const char * msg, ...);
  void (*error)(const char * msg, ...);
  void (*fatal)(const char * msg, ...);

  void (*log)(const char * level, const char * msg, ...);
  void (*vlog)(const char * level, const char * msg, va_list ap);
} CMAP_LOG;

typedef struct
{

  const char * debug;
  const char * info;
  const char * warn;
  const char * error;
  const char * fatal;

  CMAP_LOG * (*init)();
  CMAP_LOG * (*instance)();
} CMAP_LOG_PUBLIC;

extern const CMAP_LOG_PUBLIC cmap_log_public;

#endif
