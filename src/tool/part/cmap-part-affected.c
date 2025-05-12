
#include "cmap-part-affected.h"

#include "cmap-part-keys.h"

/*******************************************************************************
*******************************************************************************/

static void add(const char * map, const char * name,
  CMAP_PART_CTX_BLOCK * block)
{
  cmap_part_keys_public.add(&block -> affecteds, map, name);
}

/*******************************************************************************
*******************************************************************************/

static char contains_n_add(const char * map, const char * name,
  CMAP_PART_CTX_BLOCK * block)
{
  if(!cmap_part_keys_public.contains(block -> affecteds, map, name))
  {
    add(map, name, block);
    return (1 == 0);
  }
  else return (1 == 1);
}

/*******************************************************************************
*******************************************************************************/

const CMAP_PART_AFFECTED_PUBLIC cmap_part_affected_public = {add,
  contains_n_add};
