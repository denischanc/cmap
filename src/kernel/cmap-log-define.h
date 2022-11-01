#ifndef __CMAP_LOG_DEFINE_H__
#define __CMAP_LOG_DEFINE_H__

#define CMAP_LOG_LOOP(macro) \
  macro(DEBUG, debug) \
  macro(INFO, info) \
  macro(WARN, warn) \
  macro(ERROR, error) \
  macro(FATAL, fatal)

#define CMAP_LOG_FN(LVL, lvl) \
  void (*lvl)(const char * msg, ...); \
  void (*v##lvl)(const char * msg, va_list ap);

#define CMAP_LOG_LEVEL(LVL, lvl) extern const char * CMAP_LOG_##LVL;

#endif
