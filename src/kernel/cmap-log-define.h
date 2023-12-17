#ifndef __CMAP_LOG_DEFINE_H__
#define __CMAP_LOG_DEFINE_H__

#define CMAP_LOG_DECL(LVL, lvl, i) \
  void (*lvl)(const char * msg, ...); \
  void (*v##lvl)(const char * msg, va_list args);

#endif
