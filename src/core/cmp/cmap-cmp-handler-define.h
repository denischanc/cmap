#ifndef __CMAP_CMP_HANDLER_DEFINE_H__
#define __CMAP_CMP_HANDLER_DEFINE_H__

#define CMAP_CMP_HANDLER_LOOP(macro) \
  macro(string) \
  macro(int) \
  macro(double)

#define CMAP_CMP_HANDLER_DECL(type) \
  extern const CMAP_CMP_HANDLER cmap_cmp_handler_##type;

#endif
