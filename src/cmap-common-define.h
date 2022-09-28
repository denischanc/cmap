#ifndef __CMAP_COMMON_DEFINE_H__
#define __CMAP_COMMON_DEFINE_H__

#define CMAP_CALL(e, fn) (e) -> fn(e)
#define CMAP_CALL_ARGS(e, fn, args...) (e) -> fn(e, args)

#endif
