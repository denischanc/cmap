
#include "cmap-clean.h"

#include "cmap-part.h"
#include "cmap-fn-name.h"
#include "cmap-parser-block.h"

/*******************************************************************************
*******************************************************************************/

static void clean()
{
  cmap_part_public.clean();
  cmap_parser_block_public.clean();
}

/*******************************************************************************
*******************************************************************************/

const CMAP_CLEAN_PUBLIC cmap_clean_public = { clean };
