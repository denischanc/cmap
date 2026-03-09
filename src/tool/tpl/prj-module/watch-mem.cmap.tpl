@INCLUDE
{
#include "watch-mem-fn.h"
}

new cmap.job(function()(watch_mem_fn), function()(watch_mem_stop_fn))
  .daemonizeMs(0, 500);
