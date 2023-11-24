#ifndef __CMAP_FN_NAME_DEFINE_H__
#define __CMAP_FN_NAME_DEFINE_H__

#define CMAP_FN_NAME_LOOP(macro) \
  macro(option, (1 == 0)) \
  macro(basename_no_suffix, (1 == 1))

#define CMAP_FN_NAME_DECL(from, format) void (*from_##from)(const char * name);

#endif
