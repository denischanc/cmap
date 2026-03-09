
#include "watch-mem-fn.h"

#include <stdio.h>
#include <stddef.h>
#include <cmap/cmap-mem-ext.h>

static FILE * watch_mem_file = NULL;

static int id = 0;

CMAP_MAP * watch_mem_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if(watch_mem_file == NULL)
  {
    watch_mem_file = fopen("mem.dat", "w");
    fprintf(watch_mem_file, "time alloc free chunk block block-free\n");
  }

  CMAP_MEM_STATE * state = cmap_mem_state();
  fprintf(watch_mem_file, "%d %d %d %d %d %d\n",
    id++ * 5, state -> size_alloc, state -> size_free, state -> nb_chunk,
    state -> nb_block, state -> nb_block_free);

  return NULL;
}

CMAP_MAP * watch_mem_stop_fn(CMAP_PROC_CTX * proc_ctx, CMAP_MAP * map,
  CMAP_LIST * args)
{
  if(watch_mem_file != NULL)
  {
    fprintf(watch_mem_file, "# the end\n");
    fclose(watch_mem_file);
  }

  return NULL;
}
