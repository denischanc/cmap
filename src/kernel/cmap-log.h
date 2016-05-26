#ifndef __CMAP_LOG_H__
#define __CMAP_LOG_H__

#include <stdarg.h>

extern const char * CMAP_LOG_DEBUG;
extern const char * CMAP_LOG_INFO;
extern const char * CMAP_LOG_WARN;
extern const char * CMAP_LOG_ERROR;
extern const char * CMAP_LOG_FATAL;

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

void cmap_log_init();

#endif
