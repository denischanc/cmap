#ifndef __CMAP_LOG_DEFINE_EXT_H__
#define __CMAP_LOG_DEFINE_EXT_H__

#define CMAP_LOG_LOOP(macro) \
  macro(DEBUG, debug, 0) \
  macro(INFO, info, 1) \
  macro(WARN, warn, 2) \
  macro(ERROR, error, 3) \
  macro(FATAL, fatal, 4)

#define CMAP_LOG_CONST_DECL(LVL, lvl, i) extern const char CMAP_LOG_##LVL;

#endif
