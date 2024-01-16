
#include "cmap-assert.h"

#include "cmap-log.h"
#include "cmap-kernel.h"

/*******************************************************************************
*******************************************************************************/

static void assert_(char cond, const char * path, int line,
  const char * msg_err)
{
  if(!cond)
  {
    cmap_log_public.fatal("[%s:%d] %s", path, line, msg_err);
    CMAP_KERNEL_INSTANCE -> fatal();
  }
}

/*******************************************************************************
*******************************************************************************/

const CMAP_ASSERT_PUBLIC cmap_assert_public = { assert_ };
