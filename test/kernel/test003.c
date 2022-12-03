/*******************************************************************************
@TEST_DESC@ Call console info/error fns
*******************************************************************************/

#include <stdlib.h>
#include "cmap-ext.h"

/*******************************************************************************
*******************************************************************************/

int main(int argc, char * argv[])
{
  cmap_bootstrap(NULL);

  cmap_env_main(cmap_env(), argc, argv, NULL,
    "local test = function()/t/{"
    "  cmap.console.info(\"Hello \"/t/, \"World \"/t/, \"!!!\"/t/).info();"
    "  cmap.console.info();"
    "};"
    "local job = new cmap.scheduler.job(test)/t/;"
    "job.schedule();");

  return cmap_main();
}
