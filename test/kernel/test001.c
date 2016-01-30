
#include "cmap-kernel.h"

#include "test-assert.h"
#include <stdlib.h>

int main(int argc, char * argv[])
{
  cmap_kernel_create(NULL);

  CMAP_TEST_ASSERT(cmap_kernel() -> start(argc, argv) == EXIT_SUCCESS,
    "Bad kernel return value");

  return EXIT_SUCCESS;
}
