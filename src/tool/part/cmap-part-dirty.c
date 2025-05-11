
#include "cmap-part-dirty.h"

#include "cmap-strings.h"

/*******************************************************************************
*******************************************************************************/

static void name(const char * name, CMAP_PART_CTX * ctx)
{
  CMAP_PART_CTX_BLOCK * block = &ctx -> block;

  if(block -> nature == CMAP_PART_CTX_NATURE_BLOCK)
    cmap_strings_public.add(&block -> dirties, name);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_DIRTY_PUBLIC cmap_part_dirty_public = {name};
