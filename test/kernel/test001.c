/*******************************************************************************
@TEST_DESC@ Instantiate kernel
*******************************************************************************/

#include "cmap-kernel.h"

#include <stdlib.h>

int main(int argc, char * argv[])
{
  cmap_kernel_init(NULL);
  return cmap_kernel() -> main(argc, argv);
}
