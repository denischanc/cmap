
#include "cmap-kernel.h"

#include <stdlib.h>

int main(int argc, char * argv[])
{
  cmap_kernel_create(NULL);
  return cmap_kernel() -> main(argc, argv);
}
