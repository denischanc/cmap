/*******************************************************************************
@TEST_DESC@ Instantiate kernel
*******************************************************************************/

#include <cmap/cmap.h>

#include <stdlib.h>

int main(int argc, char * argv[])
{
  cmap_bootstrap(NULL);
  return cmap_main(argc, argv, NULL);
}
