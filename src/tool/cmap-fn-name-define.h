#ifndef __CMAP_FN_NAME_DEFINE_H__
#define __CMAP_FN_NAME_DEFINE_H__

#define CMAP_FN_NAME_LOOP(macro) \
  macro(parser) \
  macro(option) \
  macro(path)

#define CMAP_FN_NAME_DECL(from) void (*from_##from)(const char * name);

#endif
