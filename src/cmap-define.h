#ifndef __CMAP_DEFINE_H__
#define __CMAP_DEFINE_H__

#define CMAP_CALL(e, fn) (e) -> fn(e)
#define CMAP_CALL_ARGS(e, fn, args...) (e) -> fn(e, args)

#define CMAP_APPLY(e, args...) CMAP_CALL_ARGS(e, apply, args)
#define CMAP_DELETE(e) CMAP_CALL(e, delete)

#define CMAP_UNUSED __attribute__((unused))

#endif
