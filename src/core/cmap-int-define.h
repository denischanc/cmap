#ifndef __CMAP_INT_DEFINE_H__
#define __CMAP_INT_DEFINE_H__

#define CMAP_INT(val, proc_ctx, aisle) \
  cmap_int_public.create(val, proc_ctx, aisle)

#define CMAP_INT_OP_LOOP(macro) \
  macro(set, =) \
  macro(add, +=) \
  macro(sub, -=) \
  macro(mul, *=) \
  macro(div, /=)

#define CMAP_INT_OP_DECL(name, op) \
  CMAP_INT * (*name)(CMAP_INT * this, int64_t val);

#define CMAP_INT_STEP_LOOP(macro) \
  macro(inc, ++) \
  macro(dec, --)

#define CMAP_INT_STEP_DECL(name, op) CMAP_INT * (*name)(CMAP_INT * this);

#endif
