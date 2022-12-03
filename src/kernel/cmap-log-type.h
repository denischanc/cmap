#ifndef __CMAP_LOG_TYPE_H__
#define __CMAP_LOG_TYPE_H__

#include <stdarg.h>

typedef struct
{
  void (*log)(const char * level, const char * msg, ...);
  void (*vlog)(const char * level, const char * msg, va_list ap);
} CMAP_LOG;

#endif
