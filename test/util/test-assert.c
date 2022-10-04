
#include "test-assert.h"

#include <stdio.h>
#include <stdlib.h>
#include "cmap-kernel.h"

void cmap_test_assert(char cond, const char * file, int line,
  const char * msg)
{
  if(!cond)
  {
    printf("[%s:%i][TEST] %s => KO\n", file, line, msg);
    if(cmap_kernel_public.this() != NULL) cmap_kernel_public.this() -> fatal();
    else exit(EXIT_FAILURE);
  }
  else
  {
    printf("[%s:%i][TEST] %s => OK\n", file, line, msg);
  }
}
