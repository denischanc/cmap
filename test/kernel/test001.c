/*******************************************************************************
@TEST_DESC@ Instantiate kernel
*******************************************************************************/

#include <cmap/cmap.h>

#include <stdlib.h>

int main(int argc, char * argv[])
{
  cmap_bootstrap(NULL);
  cmap_env_main(cmap_env(), argc, argv, NULL, "");
  return cmap_main();
}
