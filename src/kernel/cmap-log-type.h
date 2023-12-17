#ifndef __CMAP_LOG_TYPE_H__
#define __CMAP_LOG_TYPE_H__

#include <stdarg.h>

typedef struct
{
  void (*vlog)(const char * level, const char * msg, va_list args);
} CMAP_LOG;

#endif
