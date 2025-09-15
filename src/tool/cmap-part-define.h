#ifndef __CMAP_PART_DEFINE_H__
#define __CMAP_PART_DEFINE_H__

#define CMAP_PART_LOOP(macro) \
  macro(includes) \
  macro(main) \
  macro(headers)

#define CMAP_PART_DECL(part) char ** (*part)();

#endif
