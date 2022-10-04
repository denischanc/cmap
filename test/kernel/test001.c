/*******************************************************************************
@TEST_DESC@ Instantiate kernel
*******************************************************************************/

#include "cmap.h"

#include <stdlib.h>

int main(int argc, char * argv[])
{
  cmap_init(NULL);
  return cmap_main(argc, argv);
}
