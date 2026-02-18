#ifndef __CMAP_LOG_DEFINE_H__
#define __CMAP_LOG_DEFINE_H__

#define CMAP_LOG_LOOP(macro) \
  macro(DEBUG, debug) \
  macro(INFO, info) \
  macro(WARN, warn) \
  macro(ERROR, error) \
  macro(FATAL, fatal)

#define CMAP_LOG_DECL(LVL, lvl) \
  void cmap_log_##lvl(const char * msg, ...); \
  void cmap_log_v##lvl(const char * msg, va_list args);

#endif
