
#include "test-assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <cmap/cmap.h>

void cmap_test_assert(char cond, const char * file, int line,
  const char * msg)
{
  if(!cond)
  {
    printf("[%s:%i][TEST] %s => KO\n", file, line, msg);
    cmap_fatal();
  }
  else
  {
    printf("[%s:%i][TEST] %s => OK\n", file, line, msg);
  }
}
