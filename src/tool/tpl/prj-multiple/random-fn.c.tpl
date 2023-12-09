
#include "random-fn.h"

#include <stdlib.h>
#include <time.h>
#include <cmap/cmap-ext.h>
#include <cmap/cmap-int-ext.h>

static char srand_ok = CMAP_F;

CMAP_MAP * random_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  CMAP_MAP * i = cmap_list_get(args, 0);
  if((i != NULL) && (cmap_nature(i) == CMAP_INT_NATURE))
  {
    if(!srand_ok)
    {
      srand(time(NULL));
      srand_ok = CMAP_T;
    }

    int ret = rand();
    CMAP_MAP * max = cmap_list_get(args, 1);
    if((max != NULL) && (cmap_nature(max) == CMAP_INT_NATURE))
      ret = ret % cmap_int_get((CMAP_INT *)max);
    cmap_int_set((CMAP_INT *)i, ret);
  }

  return i;
}
