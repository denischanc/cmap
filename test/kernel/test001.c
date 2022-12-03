/*******************************************************************************
@TEST_DESC@ Instantiate kernel
*******************************************************************************/

#include <stdlib.h>
#include "cmap-ext.h"

int main(int argc, char * argv[])
{
  cmap_bootstrap(NULL);
  cmap_env_main(cmap_env(), argc, argv, NULL, "");
  return cmap_main();
}
