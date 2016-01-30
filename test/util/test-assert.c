
#include "test-assert.h"

#include <stdio.h>
#include <stdlib.h>

void cmap_test_assert(char cond, const char * file, int line,
  const char * msg)
{
  if(!cond)
  {
    fprintf(stderr, "[%s:%i][ERROR]  %s\n", file, line, msg);
    exit(EXIT_FAILURE);
  }
}
