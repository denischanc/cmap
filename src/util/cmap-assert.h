#ifndef __CMAP_ASSERT_H__
#define __CMAP_ASSERT_H__

#include "cmap-assert-define.h"

typedef struct
{
  void (*assert)(char cond, const char * path, int line, const char * msg_err);
} CMAP_ASSERT_PUBLIC;

extern const CMAP_ASSERT_PUBLIC cmap_assert_public;

#endif
