#ifndef __CMAP_ASSERT_DEFINE_H__
#define __CMAP_ASSERT_DEFINE_H__

#define CMAP_ASSERT(cond, msg_err) \
  cmap_assert_public.assert(cond, __FILE__, __LINE__, msg_err)

#endif
