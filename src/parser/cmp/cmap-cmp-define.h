#ifndef __CMAP_CMP_DEFINE_H__
#define __CMAP_CMP_DEFINE_H__

#include "cmap-int-type.h"
#include "cmap-map-type.h"
#include "cmap-proc-ctx-type.h"

#define CMAP_CMP_LOOP(macro) \
  macro(lt, <) \
  macro(gt, >) \
  macro(le, <=) \
  macro(ge, >=) \
  macro(equal, ==) \
  macro(diff, !=)

#define CMAP_CMP_DECL(name, cmp) \
  CMAP_INT * (*name)(CMAP_MAP * map_l, CMAP_MAP * map_r, \
    CMAP_PROC_CTX * proc_ctx);

#endif
