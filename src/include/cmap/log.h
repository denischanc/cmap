#ifndef ___CMAP_LOG_H___
#define ___CMAP_LOG_H___

#include <stdarg.h>

typedef struct
{
  void (*log)(const char * level, const char * msg, ...);
  void (*vlog)(const char * level, const char * msg, va_list ap);
} CMAP_LOG;

#endif
