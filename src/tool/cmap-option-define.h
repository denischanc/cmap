#ifndef __CMAP_OPTION_DEFINE_H__
#define __CMAP_OPTION_DEFINE_H__

#define CMAP_OPTION_LOOP(macro_bool, macro_string) \
  macro_bool(relative_inc) \
  macro_string(fn_name)

#define CMAP_OPTION_BOOL_DECL(name) \
  void (*name)(); \
  char (*is_##name)();

#define CMAP_OPTION_STRING_DECL(name) \
  void (*set_##name)(char * val); \
  char * (*name)();

#endif
